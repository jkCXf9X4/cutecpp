
#pragma once

namespace cutecpp
{
    /**
     * @brief Severity levels used by the ::Logger class.
     */
    enum class LogLevel : int
    {
        ext_trace, /**< Extremely verbose trace messages */
        trace,     /**< Verbose tracing information */
        debug,     /**< Debug information */
        info,      /**< General information */
        success,   /**< Successful operations */
        warning,   /**< Warnings */
        error,     /**< Error conditions */
        fatal      /**< Fatal errors */
    };
}
