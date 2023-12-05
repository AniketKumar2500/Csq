#!/usr/bin/env python3
# -*- coding: utf-8 -*-

""" Program entry point
This file is to be treated as an entry point to the program
Arguments passed to the program are checked here
"""

from sys import argv as arguments, version_info
from os import getenv, system, getcwd as pwd, path
from Compiler.Compiletime.wrapper import bind
from Compiler.Parser.parser import Compile
from Compiler.code_format import readCode, toTokens, writeCode

VERSION = "4.3"

def check_python_version():
    """Check python version
        Check if the python version is 3.10 or above
        If not, print an error message and exit
    """

    if version_info.major < 3:
        print("Error: Python version 3.10 or above required")
        exit(1)

    if version_info.minor < 10:
        print("Error: Python version 3.10 or above required")
        exit(1)

def printHelp(exit_code = 1):
    """Print help
        Print the help message for the program
    """

    print("Usage: csq [options] <file>")
    print("Options:")
    print("  This options are to be used when compiling the cpp code generated by csq")
    print("  -h, --help\t\t\tPrint this help message")
    print("  -v, --version\t\t\tPrint the version of csq")
    print("  -u, --uninstall\t\tUninstall the csq")
    print("  -k, --keep\t\t\tKeep the generated cpp file")
    print("  -o, --output <file>\t\tSpecify the output file")
    print("  -O, --optimize <level>\tSpecify the optimization level")
    print("  -c, --compile\t\t\tCompile the file and generate object file")
    print("  -S, --assembly\t\tGenerate assembly code")
    print("  -E, --preprocess\t\tGenerate preprocessed code")
    print("  -g, --debug\t\t\tGenerate debug symbols")

    exit(exit_code)

def isFileValid(file):
    """Check if the file is valid
        Check if the arguments passed are enough or and valid
    """

    if len(arguments) < 2:
        printHelp()
        return False

    # Check if the file exists
    try:
        open(file, "r")
    except FileNotFoundError:
        printHelp()
        return False

    return True

def findIncludePath():
    """Find the include path
        The function finds the include path for the csq compiler
        Check for predefined locations instead of just relying on environment variables
        Sometimes the environment variables are not set (or shell incorrectly configured)

    Check for the following locations:
        1. $HOME/.local/include/csq
        2. /usr/local/include/csq
        3. /opt/csq
        4. /usr/include/csq
        4. Current directory (Maybe a developer is testing the compiler)
    """

    # Check for $HOME/.csq/include/csq
    home = getenv("HOME")
    if home is not None:
        csq_include_path = home + "/.local/include/csq"
        if path.exists(csq_include_path):
            return csq_include_path

    # Check for /usr/local/include/csq
    if path.exists("/usr/local/include/csq"):
        return "/usr/local/include/csq"

    # Check for /opt/csq
    if path.exists("/opt/csq"):
        return "/opt/csq"

    # Check for /usr/include/csq
    if path.exists("/usr/include/csq"):
        return "/usr/include/csq"

    # Check for current directory
    if path.exists(pwd()):
        return pwd()

    # If none of the above locations are found, return None
    return None

def compileFile(file, options, keep = False):
    """Compile the code in a file
        The function takes in a file as string and compiles it.
        The compiled code is stored in a file with the same name
    """
    # csq include path path
    csq_include_path = findIncludePath()

    if csq_include_path is None:
        print("Error: csq include path not found")
        exit(1)

    # Read the file and process it
    raw_code = readCode(file)

    # Convert it into stream of tokens
    lines = toTokens(raw_code)

    # Moving forth to compilation
    compiled_code = Compile(lines)

    # cpp file
    cpp_file = file.replace(".csq", ".cpp")

    final_code = bind(csq_include_path, compiled_code)
    writeCode(final_code, cpp_file)

    print("g++ {} {}".format(options, cpp_file))
    system("g++ {} {}".format(options, cpp_file))
    if not keep:
        system("rm {}".format(cpp_file))

# Uninstall instructions for csq
def uninstall():
    """Uninstall csq
        The function uninstalls the csq compiler
        The function removes the csq executable and the csq include path

        First remove the include directory if available
        Then check for the executable and remove it
        The executable should be accessible from the PATH environment variable
        Use the whereis command to find the executable, this should return an array of paths
        Remove the paths (if any)
        If the include directory is the current directory ignore as this is a developer environment
    """

    # csq include path path
    csq_include_path = findIncludePath()

    if csq_include_path is None:
        print("Error: csq include path not found")

    # Remove the include path
    if csq_include_path != pwd():
        system("rm -rf {}".format(csq_include_path))

    # Remove the executable
    paths = system("whereis csq")
    # Check if the paths are returned
    if paths == 0:
        print("Error: csq executable not installed, nothing to uninstall")
        exit(1)
    paths = paths.split(" ")
    for path in paths:
        if path != "csq:":
            system("rm {}".format(path))

    print("csq uninstalled successfully")


# Before argument parsing and anything use a crude way to take in arguments
# These are just a small subset of arguments that are to be accepted
def handleArguments():
    """Handle arguments
        The function handles the arguments passed to the program
        The arguments are handled in a crude way
    """

    # Check how many arguments are passed
    args_passed = len(arguments)
    isFilePassed = False
    keepFile = False
    file_name = ""

    options = ["-h", "--help", "-v", "--version", "-u", "--uninstall", "-k", "--keep", "-o", "--output", "-O", "--optimize", "-c", "--compile", "-S", "--assembly", "-E", "--preprocess", "-g", "--debug"]
    compiler_flags = "-std=c++17" # Default compiler flags

    # The first argument passed should be either a filename or an option that doesn't require a filename

    if args_passed == 1:
        printHelp(1)
    else:
        # Find the filename by checking all the arguments passed and checking if it is a file
        # If more than one the first one with .csq extension is taken
        for arg in arguments:
            if arg.endswith(".csq") and arg not in options:
                file_name = arg
                isFilePassed = True
                break

    # Populate the compiler flags
    if "-O" in arguments or "--optimize" in arguments:
        index = arguments.index("-O") if "-O" in arguments else arguments.index("--optimize")
        compiler_flags += " -O" + arguments[index + 1]

    if "-g" in arguments or "--debug" in arguments:
        compiler_flags += " -g"

    if "-c" in arguments or "--compile" in arguments:
        compiler_flags += " -c"

    if "-S" in arguments or "--assembly" in arguments:
        compiler_flags += " -S"

    if "-E" in arguments or "--preprocess" in arguments:
        compiler_flags += " -E"

    if "-o" in arguments or "--output" in arguments:
        index = arguments.index("-o") if "-o" in arguments else arguments.index("--output")
        compiler_flags += " -o " + arguments[index + 1]
    elif ("-c" in arguments or "--compile" in arguments) and "-o" not in arguments and "--output" not in arguments and file_name != "":
        compiler_flags += " -o " + file_name.replace(".csq", ".o")
    else:
        compiler_flags += " -o " + file_name.replace(".csq", "")

    # Handle the options
    if "-h" in arguments or "--help" in arguments:
        printHelp(0)
    elif "-v" in arguments or "--version" in arguments:
        print("csq version {}".format(VERSION))
        exit(0)
    elif "-u" in arguments or "--uninstall" in arguments:
        uninstall()
    elif "-k" in arguments or "--keep" in arguments:
        keepFile = True

    # If the file is passed, compile it
    if isFilePassed and isFileValid(file_name):
        compileFile(file_name, compiler_flags, keepFile)
    else:
        printHelp(1)


if __name__ == "__main__":
    # Handle the arguments passed
    check_python_version()
    handleArguments()
