#pragma once


#include <iostream>
#include <mutex>
#include <string>
#include <sstream>
#include <vector>


namespace kctf {

class LoggerT {
private:
    using OutputStreamT = std::ostream;
    using StringT = std::string;
    using CodeT = StringT;

public:
    enum class Level {
        none = 0,
        error = 2,
        warning = 4,
        info = 6,
        debug = 10,
        trace = 20
    };

    enum class Align {
        left = -1,
        middle = 0,
        right = 1
    };

    struct ParenthesisT {
        StringT left_;
        StringT right_;

        bool operator!=(const ParenthesisT &other) const {
            return left_ != other.left_ || right_ != other.right_;
        }
    };

    class LoggerStreamT {
    private:
        class ProxyT {
        private:
            LoggerStreamT &logger_stream_;
            std::stringstream ss_;

            std::vector<CodeT> additional_codes_;

        public:
            ProxyT(LoggerStreamT &logger_stream, const std::vector<CodeT> &additional_codes = {});
            ~ProxyT();

            ProxyT(ProxyT &);
            ProxyT(ProxyT &&);

            template <typename T>
            ProxyT &operator<<(const T &value) {
                ss_ << value;
                return *this;
            }
        };

        class AdditionalCodeProxyT {
        private:
            LoggerStreamT &logger_stream_;
            std::vector<CodeT> additional_codes_;

        public:
            AdditionalCodeProxyT(LoggerStreamT &logger_stream, const std::vector<CodeT> &additional_codes);
            AdditionalCodeProxyT operator()(const CodeT &code);

            template <typename T>
            ProxyT operator<<(const T &value) {
                ProxyT proxy(logger_stream_, additional_codes_);
                proxy << value;
                return proxy;
            }
        };

    private:
        LoggerT &logger_;
        Level log_level_;
        std::vector<CodeT> codes_;
        std::vector<ParenthesisT> parentheses_;
        StringT split_;
        StringT end_;
        bool to_ignore_prefix_;
        bool to_print_codes_anyway_;

    public:
        bool to_replace_codes_ = false;

    public:
        LoggerStreamT(LoggerT &logger,
                      Level log_level,
                      const std::vector<CodeT> &codes,
                      const std::vector<ParenthesisT> &parentheses=LoggerT::default_parentheses_,
                      const StringT &split=" : ",
                      const StringT &end="\n",
                      bool to_ignore_prefix=false,
                      bool to_print_codes_anyway=true);

        AdditionalCodeProxyT operator()(const CodeT &additional_code);

        template <typename T>
        ProxyT operator<<(const T &value) {
            ProxyT proxy(*this);
            proxy << value;
            return proxy;
        }

        void flush();
        void n();

        StringT get_split() const;
        StringT get_end() const;
        bool get_to_ignore_prefix() const;

        void set_parentheses(const std::vector<ParenthesisT> &parentheses);
        void set_split(const StringT &split);
        void set_end(const StringT &end);
        void set_to_ignore_prefix(bool to_ignore_prefix);

        bool is_tty() const;
    };

private:
    std::mutex mutex_;
    
    OutputStreamT &stream_;
    Level log_level_;

    bool is_tty_ = true;

public:
    const static std::vector<ParenthesisT> default_parentheses_;

private:
    struct CodeEntryT {
        CodeT code_;
        size_t max_code_length_;

        ParenthesisT parentheses_;
        size_t max_par_length_left;
        size_t max_par_length_right;
    };

    std::vector<CodeEntryT> used_codes_;

private:
    bool used_code_fully_mathches(const std::vector<CodeT> &codes, const std::vector<ParenthesisT> &parenthesis) const;
    void print_code(size_t code_index, const CodeT &code, const ParenthesisT &parentheses, bool to_print_parentheses_anyway=false, bool to_print_codes_anyway=false);
    void print_aligned(const StringT &str, Align align, size_t width);

public:
    void print(
        Level level,
        const std::vector<CodeT> &codes,
        const StringT &message,
        const StringT &split=" : ",
        const StringT &end="\n",
        bool to_ignore_prefix=false,
        const std::vector<ParenthesisT> &parentheses=default_parentheses_,
        bool to_print_codes_anyway=true
    );

    void set_log_level(Level level);

    void flush();

    LoggerStreamT error;
    LoggerStreamT warning;
    LoggerStreamT info;
    LoggerStreamT debug;
    LoggerStreamT trace;

    LoggerStreamT nc_error;
    LoggerStreamT nc_warning;
    LoggerStreamT nc_info;
    LoggerStreamT nc_debug;
    LoggerStreamT nc_trace;

public:
    LoggerT(Level log_level=Level::info, OutputStreamT &stream=std::cout, bool is_tty=true);

    bool is_tty() const;
    void set_tty(bool is_tty);

    OutputStreamT &get_stream();
};

extern LoggerT logger;

} // namespace kctf

using kctf::logger;
