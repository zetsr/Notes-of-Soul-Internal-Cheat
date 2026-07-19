#pragma once

#include <string>
#include <string_view>

namespace xor_util {

    template <typename CharT>
    inline size_t get_len(const CharT* str) {
        size_t len = 0;
        while (str[len] != 0) { len++; }
        return len;
    }

    class RuntimeXorStr {
    public:
        template <typename CharT>
        inline RuntimeXorStr(const CharT* str, const char* key) noexcept {
            m_size = get_len(str);
            m_key_len = get_len(key);

            for (size_t i = 0; i < m_size && i < 128; ++i) {
                m_data[i] = static_cast<char>(str[i]) ^ key[i % m_key_len];
            }
            for (size_t i = 0; i < m_key_len && i < 32; ++i) {
                m_key[i] = key[i];
            }
        }

        // 运行时在栈上瞬间解密，渲染完这一行代码后临时 string 自动析构释放内存
        [[nodiscard]] inline std::string str() const {
            std::string out;
            out.reserve(m_size);
            for (size_t i = 0; i < m_size; ++i) {
                out.push_back(m_data[i] ^ m_key[i % m_key_len]);
            }
            return out; // 正常返回，绝不提前擦除
        }

    private:
        char m_data[128]{};
        char m_key[32]{};
        size_t m_size = 0;
        size_t m_key_len = 0;
    };

} // namespace xor_util

#define _XOR_(str) ([]() -> ::xor_util::RuntimeXorStr { \
    static const ::xor_util::RuntimeXorStr instance(str, "0XJ2X82NX8Na0xn2xX_1X1?X2_X2!!Z2____X2"); \
    return instance; \
}())