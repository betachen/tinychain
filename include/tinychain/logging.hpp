/**
 * Part of:
 * Comments:
 *
**/
/**
 * Copyright (c) 2011-2015 libbitcoin developers (see AUTHORS)
 * Copyright (c) 2016-2018 metaverse core developers (see MVS-AUTHORS)
 *
 * This file is part of metaverse.
 *
 * metaverse is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License with
 * additional permissions to the one published by the Free Software
 * Foundation, either version 3 of the License, or (at your option)
 * any later version. For more information see LICENSE.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef MVS_NETWORK_LOGGING_HPP
#define MVS_NETWORK_LOGGING_HPP

#include <fstream>
#include <iostream>
#include <functional>
#include <map>
#include <sstream>
#include <string>

namespace tinychain {

class log
{
public:
    enum class level
    {
    	trace,
        debug,
        info,
        warning,
        error,
        fatal,
        null
    };

    typedef std::function<void(level, const std::string&, const std::string&)>
        functor;

    log(level value, const std::string& domain);
    log(log&& other);
    ~log();

    /// Clear all log configuration.
    static void clear();

    /// Convert the log level value to English text.
    static std::string to_text(level value);

    // Stream to these functions.
    static log trace(const std::string& domain);
    static log debug(const std::string& domain);
    static log info(const std::string& domain);
    static log warning(const std::string& domain);
    static log error(const std::string& domain);
    static log fatal(const std::string& domain);

    template <typename Type>
    log& operator<<(Type const& value)
    {
        stream_ << value;
        return *this;
    }

    /// Set the output functor for this log instance.
    void set_output_function(functor value);

private:
    typedef std::map<level, functor> destinations;

    static void output_ignore(level value, const std::string& domain,
        const std::string& body);
    static void output_cout(level value, const std::string& domain,
        const std::string& body);
    static void output_cerr(level value, const std::string& domain,
        const std::string& body);

    static void to_stream(std::ostream& out, level value,
        const std::string& domain, const std::string& body);

    static destinations destinations_;

    level level_;
    std::string domain_;
    std::ostringstream stream_;
};


/// Set up global logging.
void initialize_logging(std::ofstream& debug, std::ofstream& error,
    std::ostream& output_stream, std::ostream& error_stream, std::string level = "DEBUG");

/// Class Logger
class Logger{
#define self Logger

public:
    self() noexcept
    {
        initialize_logging(debug_log_, error_log_, std::cout, std::cerr);
    }

	self(const self&) = delete;
	self(const self&&) = delete;

    ~self() noexcept
    {
        log::clear();
		debug_log_.close();
		error_log_.close();
    }

public:
/// Constant for logging file open mode (append output).
static constexpr std::ofstream::openmode log_open_mode =
    std::ofstream::out | std::ofstream::app;

private:
    std::ofstream debug_log_{"debug.log", log_open_mode};
    std::ofstream error_log_{"error.log", log_open_mode};

#undef self
};// class Logger

} // namespace libbitcoin

#endif
