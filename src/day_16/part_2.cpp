#include <fstream>
#include <filesystem>
#include <iostream>
#include <vector>
#include <array>
#include <variant>
#include <memory>
#include <stdexcept>
#include <algorithm>
#include <numeric>

struct HexidecmalByte : std::array<char, 3>
{
    inline friend auto &operator>>(std::istream &is, HexidecmalByte &rhs)
    {
        rhs[2] = '\0';
        return is >> rhs[0] >> rhs[1];
    }

    operator uint8_t()
    {
        return static_cast<uint8_t>(std::stoul(data(), 0, 16));
    }
};

struct Packet
{
    constexpr Packet(uint8_t ver, uint64_t val) : version(ver), value(val) {}
    constexpr virtual ~Packet(){};
    uint8_t version;
    uint64_t value;

    inline friend auto operator+(const Packet &lhs, const Packet &rhs) noexcept
    {
        return lhs.value + rhs.value;
    }

    inline friend auto operator*(const Packet &lhs, const Packet &rhs) noexcept
    {
        return lhs.value * rhs.value;
    }
};

struct OperatorPacket : Packet
{
    template <typename SubpacketsType>
    constexpr OperatorPacket(uint8_t ver, uint64_t val, SubpacketsType &&s) : Packet(ver, val), subpackets(std::forward<SubpacketsType>(s)) {}
    std::vector<std::unique_ptr<Packet>> subpackets;
};

template <int count, typename Type>
Type ExtractBits(const std::vector<uint8_t> &encoded_packet, size_t &offset)
{
    static_assert(count <= std::numeric_limits<Type>::digits);
    if (offset + count > encoded_packet.size())
    {
        throw std::runtime_error(std::to_string(encoded_packet.size()));
    }

    Type temp = 0;
    for (size_t i = 0; i < count; ++i)
    {
        temp |= encoded_packet[offset++] << (count - 1 - i);
    }

    return temp;
}

std::unique_ptr<Packet> ExtractPacket(const std::vector<uint8_t> &encoded_packet, size_t &offset)
{
    auto version = ExtractBits<3, uint8_t>(encoded_packet, offset);
    auto type = ExtractBits<3, uint8_t>(encoded_packet, offset);
    if (type == 4)
    {
        uint64_t value = 0;
        std::vector<uint8_t> literal;
        uint8_t five;
        do
        {
            five = ExtractBits<5, uint8_t>(encoded_packet, offset);
            literal.push_back(five & 0b1111);
        } while ((five & 0b1'0000) != 0);

        for (size_t i = 0; i < literal.size(); i++)
        {
            value |= uint64_t(literal[i]) << ((literal.size() - 1 - i) * 4);
        }

        return std::make_unique<Packet>(version, value);
    }
    else
    {
        std::vector<std::unique_ptr<Packet>> subpackets;
        auto length_type = ExtractBits<1, uint8_t>(encoded_packet, offset);
        if (length_type == 0)
        {
            auto bit_count = ExtractBits<15, uint16_t>(encoded_packet, offset);
            auto subpackets_offset = offset + bit_count;
            while (offset < subpackets_offset)
            {
                subpackets.push_back(ExtractPacket(encoded_packet, offset));
            }
        }
        else
        {
            auto packet_count = ExtractBits<11, uint16_t>(encoded_packet, offset);
            for (size_t i = 0; i < packet_count; ++i)
            {
                subpackets.push_back(ExtractPacket(encoded_packet, offset));
            }
        }

        uint64_t value = 0;
        switch (type)
        {
        case 0:
        {
            std::for_each(subpackets.cbegin(), subpackets.cend(), [&value](const auto &subpacket)
                          { value += subpacket.get()->value; });
        }
        break;
        case 1:
        {
            value = 1;
            std::for_each(subpackets.cbegin(), subpackets.cend(), [&value](const auto &subpacket)
                          { value *= subpacket.get()->value; });
        }
        break;
        case 2:
        {
            value = -1;
            std::for_each(subpackets.cbegin(), subpackets.cend(), [&value](const auto &subpacket)
                          {
                              if (subpacket.get()->value < value)
                              {
                                  value = subpacket.get()->value;
                              }
                          });
        }
        break;
        case 3:
        {
            value = 0;
            std::for_each(subpackets.cbegin(), subpackets.cend(), [&value](const auto &subpacket)
                          {
                              if (subpacket.get()->value > value)
                              {
                                  value = subpacket.get()->value;
                              }
                          });
        }
        break;
        case 5:
        {
            value = subpackets[0]->value > subpackets[1]->value;
        }
        break;
        case 6:
        {
            value = subpackets[0]->value < subpackets[1]->value;
        }
        break;
        case 7:
        {
            value = subpackets[0]->value == subpackets[1]->value;
        }
        break;
        }
        return std::make_unique<OperatorPacket>(version, value, std::move(subpackets));
    }
}

size_t SumOfVersions(const std::unique_ptr<Packet> &packet)
{
    if (auto operator_packet = dynamic_cast<const OperatorPacket *>(packet.get()))
    {
        size_t sub_sum = 0;
        std::for_each(operator_packet->subpackets.cbegin(), operator_packet->subpackets.cend(), [&sub_sum](const std::unique_ptr<Packet> &sub_packet)
                      { sub_sum += SumOfVersions(sub_packet); });
        return operator_packet->version + sub_sum;
    }
    else
    {
        return packet.get()->version;
    }
}

int main()
{
    auto start = std::chrono::steady_clock::now();
    const auto path = std::filesystem::current_path() / "input/day_16.txt";
    std::ifstream input{path};

    std::vector<uint8_t> encoded_packet; // raw byte encoded packet
    for (HexidecmalByte hexadecimal_byte; input >> hexadecimal_byte;)
    {
        uint8_t decimal_byte = hexadecimal_byte;
        for (int i = 7; i >= 0; --i)
        {
            encoded_packet.push_back((decimal_byte & (1 << i)) != 0);
        }
    }

    size_t offset = 0;
    std::unique_ptr<Packet> decoded_packet = ExtractPacket(encoded_packet, offset);

    auto end = std::chrono::steady_clock::now();

    std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << "ms" << std::endl;
    std::cout << static_cast<uint64_t>(decoded_packet.get()->value) << std::endl;

    return 0;
}