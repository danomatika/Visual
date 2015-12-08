/*==============================================================================

	Options.h

	Copyright (C) 2015 Dan Wilcox <danomatika@gmail.com>

	This program is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program. If not, see <http://www.gnu.org/licenses/>.

==============================================================================*/
#pragma once

#include <map>
#include <vector>
#include <string>
#include <sstream>
#include <iostream>
#include "optionparser.h"

/// \class Options
/// \brief wrapper for The Lean Mean C++ Options Parser to allow dynamic creation & convenience type conversions
///
///
/// Example usage:
///
///     // create with optional program description and version strings
///     Options options("  example program which demos Options.h", "1.2.3");
///
///     // add option and argument descriptors
///     options.addString("IP", "i", "ip", "  -i, --ip \tIP address to send to (default: 127.0.0.1)");
///     options.addInteger("PORT","p", "port", "  -p, --port \tPort to send to (default: 8880)");
///     options.addSwitch("MULTICAST", "m", "multicast", "  -m, --multicast \tMulticast listening group address (off by default)");
///     options.addArgument("FILE", "  FILE \tOptional XML config file");
///
///     // parse, returns false on error or help/version prints
///     if(!options.parse(argc, argv)) {
///         return false;
///     }
///
///     // load the config file (if one exists)
///     if(options.numArguments() > 0) {
///         std::cout << "Loading " << options.getArgumentString(0) << std::endl;
///     }
///
///     // read option values if set
///     if(options.isSet("IP"))         {sendingIp = options.getString("IP");}
///     if(options.isSet("PORT"))       {sendingPort = options.getUInt("PORT");}
///     if(options.isSet("MULTICAST"))  {listeningMulticast = true;}
///
/// Example usage print:
///
///     Usage: example [options] [FILE]
///
///       example program which demos Options.h
///
///     Options:
///       -h, --help           Print usage and exit
///       --version            Print version and exit
///       -i, --ip             IP address to send to (default: 127.0.0.1)
///       -p, --port           Port to send to (default: 8880)
///       -m, --multicast      Multicast listening group address (off by default)
///
///     Arguments:
///
///       FILE                 Optional XML config file
///
/// You can also use the lower level TLMC++OP interface that takes an
/// option::Descriptor struct and enum-based option indices. See The Lean Mean
/// C++ Options Parser docs for lower level details: http://optionparser.sourceforge.net
///
/// Example code which provides the same functionality as the previous example:
///
///     // option index enum
///     enum optionNames {
///     	UNKNOWN,
///     	HELP,
///     	VERS,
///     	IP,
///     	PORT,
///     	MULTICAST
///     };
///
///     // option and usage print descriptors, note the use of the Options::Arg functions
///     // which provide extended type checks
///     const option::Descriptor usage[] = {
///     	{UNKNOWN, 0, "", "", Options::Arg::Unknown, "Options:\n"},
///     	{HELP, 0, "h", "help", Options::Arg::None, "  -h, --help \tPrint usage and exit"},
///     	{VERS, 0, "", "version", Options::Arg::None, "  --version \tPrint version and exit"},
///     	{IP, 0, "i", "ip", Options::Arg::NonEmpty, "  -i, --ip \tIP address to send to (default: 127.0.0.1)"},
///     	{PORT, 0, "p", "port", Options::Arg::Integer, "  -p, --port \tPort to send to (default: 8880)"},
///     	{MULTICAST, 0, "m", "multicast", Options::Arg::Decimal, "  -m, --multicast \tMulticast listening group address (off by default)"},
///     	{UNKNOWN, 0, "", "", Options::Arg::None, "\nArguments:\n  FILE \tOptional XML config file"},
///     	{0, 0, 0, 0, 0, 0}
///     };
///
///     // parse and check help/version options manually
///     Options options("  example program which demos Options.h");
///     if(!options.parse(usage, argc, argv)) {
///     	return false;
///     }
///     if(options.isSet(HELP)) {
///     	options.printUsage(usage, "[FILE]");
///     	return false;
///     }
///     if(options.isSet(VERSION)) {
///     	std::cout << "1.2.3" << std::endl;
///     	return false;
///     }
///
///     // load the config file (if one exists)
///     if(options.numArguments() > 0) {
///         std::cout << "Loading " << options.getArgumentString(0) << std::endl;
///     }
///
///     // read option values if set
///     if(options.isSet(IP))         {sendingIp = options.getString(IP);}
///     if(options.isSet(PORT))       {sendingPort = options.getUInt(PORT);}
///     if(options.isSet(MULTICAST))  {listeningMulticast = true;}
///
class Options {

	public:
	
		/// constructor
		/// description: optional string printed after Usage line
		/// version: optional version string, --version switch added if set
		/// note: version & help options are ignored if using the lower level
		///       parser function
		Options(std::string description="", std::string version="") : description(description), version(version), name("name") {
			indices["UKNOWN"] = 0;
			pushDescriptor(0, 0, "", "", Arg::Unknown, "Options:");
			indices["HELP"] = 1;
			pushDescriptor(1, 0, "h" , "help", Arg::None, "  -h, --help  \tPrint usage and exit");
			if(version != "") {
				indices["VERSION"] = 2;
				pushDescriptor(2, 0, "" , "version", Arg::None, "  --version  \tPrint version and exit");
			}
			pushEndDescriptor();
			parser = NULL;
			options = NULL;
			buffer = NULL;
			numRequiredNonOptions = 0;
		}
	
		/// destructor
		virtual ~Options() {
			clear();
		}
	
		/// clear current parsed options
		void clear() {
			if(parser) {delete parser;}
			if(options) {delete [] options;}
			if(buffer) {delete [] buffer;}
			parser = NULL;
			options = NULL;
			buffer = NULL;
		}
	
		/// parse commandline arguments
		/// returns true on success, false on parse error or help/version switches
		bool parse(int argc, char **argv) {
			if(!parse(descriptors.data(), argc, argv)) {
				return false;
			}
			if(options[indices["HELP"]]) {
				printUsage();
				return false;
			}
			if(options[indices["VERSION"]]) {
				std::cout << version << std::endl;
				return false;
			}
			if(parser->nonOptionsCount() < numRequiredNonOptions) {
				if(numRequiredNonOptions == 1) {
					std::cerr << "Missing required argument: " << nonOptions[0].name << std::endl;
				}
				else {
					std::cerr << "Missing required argument(s), possibly one or more of:";
					for(int i = 0; i < nonOptions.size(); ++i) {
						if(nonOptions[i].required) {
							std::cerr << " " << nonOptions[i].name;
						}
					}
					std::cerr << std::endl;
				}
				return false;
			}
			return true;
		}
	
		/// parse commandline arguments using a TLMC++OP Descriptors array (low level)
		/// returns true on success, false on parse error or help/version switches
		/// note: ignores built in options ("Options:" print, help & verison options)
		///       so you'll need to povide them manually in the array
		bool parse(const option::Descriptor usage[], int argc, char **argv) {
			clear();
			if(argc > 0) {
				name = argv[0];
				// skip program name argv[0] if present
				argc--;
				argv++;
			}
			option::Stats stats(usage, argc, argv);
			options = new option::Option[stats.options_max];
			buffer = new option::Option[stats.buffer_max];
			parser = new option::Parser(usage, argc, argv, options, buffer);
			if(parser->error()) { // print error on unknown options, parse errors, etc
				return false;
			}
			return true;
		}
	
	/// \section Add Options
	
		/// add an option which takes no value
		/// note: strings must be const (literal) or option may not be added correctly
		void addSwitch(std::string name, const char* shortopt, const char *longopt, const char *help) {
			addOption(name, 0, shortopt, longopt, Arg::None, help);
		}
	
		/// add an option which expects a string value, optionally allow empty values aka ""
		/// note: strings must be const (literal) or option may not be added correctly
		void addString(std::string name, const char* shortopt, const char *longopt, const char *help, bool allowEmpty=true) {
			addOption(name, 0, shortopt, longopt, (allowEmpty ? Arg::Required : Arg::NonEmpty), help);
		}
	
		/// add an option which expects a boolean value (0/1, yes/no, true/false)
		/// note: strings must be const (literal) or option may not be added correctly
		void addBool(std::string name, const char* shortopt, const char *longopt, const char *help) {
			addOption(name, 0, shortopt, longopt, Arg::Bool, help);
		}
	
		/// add an option which expects a numeric integer value (non-decimal)
		/// note: strings must be const (literal) or option may not be added correctly
		void addInteger(std::string name, const char* shortopt, const char *longopt, const char *help) {
			addOption(name, 0, shortopt, longopt, Arg::Integer, help);
		}
	
		/// add an option which expects a numeric decimal value
		/// note: strings must be const (literal) or option may not be added correctly
		void addDecimal(std::string name, const char* shortopt, const char *longopt, const char *help) {
			addOption(name, 0, shortopt, longopt, Arg::Decimal, help);
		}
	
		/// add a string to the help printout
		/// note: strings must be const (literal)
		void addHelp(const char *help) {
			addOption("UNKNOWN", 0, "", "", Arg::None, help);
		}
	
		/// add base TLMC++OP option (low level)
		/// note: strings must be const or option may not be added correctly
		/// note: ignores built in options ("Options:" print, help & verison options)
		///       so you'll need to provide in the array and handle them manually
		void addOption(std::string name, const int type, const char *shortopt, const char *longopt, const option::CheckArg checkArg, const char *help) {
			std::map<std::string,unsigned int>::iterator iter = indices.find(name);
			if(iter == indices.end()) {
				unsigned int index = descriptors.size()-1;
				indices[name] = index;
			}
			descriptors.pop_back();
			pushDescriptor(indices[name], 0, shortopt, longopt, checkArg, help);
			pushEndDescriptor();
		}
	
	/// \section Read Option Values Via Name
	
		/// is an option set?
		/// use this to check option existence before getting a value
		bool isSet(std::string name) {
			return options[indices[name]];
		}
	
		/// convert option from a string
		/// boolean values are 1/0, yes/no, true/false
		/// note: does not check index bounds
		bool getBool(std::string name) {
			return getBool(indices[name]);
		}
	
		/// convert option from a string
		/// note: does not check index bounds
		int getInt(std::string name) {
			return getInt(indices[name]);
		}
	
		/// convert option from a string
		/// note: does not check index bounds
		unsigned int getUInt(std::string name) {
			return getUInt(indices[name]);
		}
	
		/// convert option from a string
		/// note: does not check index bounds
		float getFloat(std::string name) {
			return getFloat(indices[name]);
		}
	
		/// convert option from a string
		/// note: does not check index bounds
		double getDouble(std::string name) {
			return getDouble(indices[name]);
		}
	
		/// get option
		/// note: does not check index bounds
		std::string getString(std::string name) {
			return options[indices[name]].last()->arg;
		}
	
		/// get the low level TLMC++OP option
		/// note: does not check index bounds
		option::Option* getOption(std::string name) {
			return options[indices[name]];
		}
	
	/// \section Read Option Values Via Index
	
		/// is an option set?
		/// use this to check option existence before getting a value
		bool isSet(unsigned int index) {
			return options[index];
		}
	
		/// convert option from a string
		/// boolean values are 1/0, yes/no, true/false
		/// note: does not check index bounds
		bool getBool(unsigned int index) {
			std::string arg = options[index].last()->arg;
			if(arg == "1" || arg == "yes" || arg == "true") {
				return true;
			}
			return false; // "0" || "no" || "false"
		}
	
		/// convert option from a string
		/// note: does not check index bounds
		int getInt(unsigned int index) {
			int i = 0;
			std::istringstream stream(options[index].last()->arg);
			stream >> i;
			return i;
		}
	
		/// convert option from a string
		/// note: does not check index bounds
		unsigned int getUInt(unsigned int index) {
			unsigned int ui = 0;
			std::istringstream stream(options[index].last()->arg);
			stream >> ui;
			return ui;
		}
	
		/// convert option from a string
		/// note: does not check index bounds
		float getFloat(unsigned int index) {
			float f = 0.f;
			std::istringstream stream(options[index].last()->arg);
			stream >> f;
			return f;
		}
	
		/// convert option from a string
		/// note: does not check index bounds
		double getDouble(unsigned int index) {
			double d = 0.0;
			std::istringstream stream(options[index].last()->arg);
			stream >> d;
			return d;
		}
	
		/// get option
		/// note: does not check index bounds
		std::string getString(unsigned int index) {
			return options[index].last()->arg;
		}
	
		/// get the low level TLMC++OP option
		/// note: does not check index bounds
		option::Option* getOption(unsigned int index) {
			return options[index];
		}
	
	/// \section Read Non-option Arguments
	
		/// add a non-option argument
		/// name is the usage name for this argument (not an index) ie. "FILE"
		/// help is the full usage help line ie. "  FILE \tXML config file"
		/// required is the non-option argument required? if so, a simple
		///           check is performed during parsing
		/// note: strings must be const (literal)
		void addArgument(const char *name, const char *help, bool required=false) {
			if(nonOptions.empty()) { // add header
				addHelp("\nArguments:\n");
			}
			addHelp(help);
			struct NonOption arg = {name, required};
			nonOptions.push_back(arg);
			if(required) {
				numRequiredNonOptions++;
			}
		}
	
		/// get the number of non-option arguments
		unsigned int numArguments() {
			return parser->nonOptionsCount();
		}
	
		/// convert a non-option argument from a string,
		/// boolean values are 1/0, yes/no, true/false
		/// note: does not check index bounds
		int getArgumentBool(unsigned int index) {
			std::string arg = parser->nonOption(index);
			if(arg == "1" || arg == "yes" || arg == "true") {
				return true;
			}
			return false; // "0" || "no" || "false"
		}
	
		/// convert a non-option argument from a string
		/// note: does not check index bounds
		int getArgumentInt(unsigned int index) {
			int i = 0;
			std::istringstream stream(parser->nonOption(index));
			stream >> i;
			return i;
		}
	
		/// convert a non-option argument from a string
		/// note: does not check index bounds
		unsigned int getArgumentUInt(unsigned int index) {
			unsigned int ui = 0;
			std::istringstream stream(parser->nonOption(index));
			stream >> ui;
			return ui;
		}
	
		/// convert a non-option argument from a string
		/// note: does not check index bounds
		float getArgumentFloat(unsigned int index) {
			float f = 0.f;
			std::istringstream stream(parser->nonOption(index));
			stream >> f;
			return f;
		}
	
		/// convert a non-option argument from a string
		/// note: does not check index bounds
		double getArgumentDouble(unsigned int index) {
			double d = 0.0;
			std::istringstream stream(parser->nonOption(index));
			stream >> d;
			return d;
		}
	
		/// get a non-option argument
		/// note: does not check index bounds
		std::string getArgumentString(unsigned int index) {
			return parser->nonOption(index);
		}
	
	/// \section Util
	
		/// print the commandline args (for debugging)
		static void print(int argc, char **argv) {
			for(int i = 0; i < argc; ++i) {
				std::cout << argv[i];
				if(i < argc-1) {
					std::cout << " ";
				}
			}
			std::cout << std::endl;
		}
	
		/// print usage, use *after* parsing otherwise program name won't be set
		/// note: is used automatically if there was a parse error
		void printUsage() {
			std::string argShortUsage = "";
			for(int i = 0; i < nonOptions.size(); ++i) {
				NonOption &n = nonOptions[i];
				if(n.required) {
					argShortUsage += "["+(std::string)n.name+"]";
				}
				else {
					argShortUsage += (std::string)n.name;
				}
				if(i < nonOptions.size()-1) {
					std::cout << " ";
				}
			}
			printUsage(descriptors.data(), argShortUsage);
		}
	
		/// print usage using a TLMC++OP Descriptor array (low level),
		/// use *after* parsing otherwise program name won't be set
		///
		/// optionally provide a short argument usage string to be printed after
		/// "Usage: prog_name [options"
		///
		/// note: is used automatically if there was a parse error
		/// note: ignores built in options ("Options:" print, help & verison options)
		///       so you'll need to povide them manually in the array
		void printUsage(const option::Descriptor usage[], std::string argShortUsage="") {
			std::cout << "Usage: " << name << " [options]";
			if(argShortUsage != "") {
				std::cout << " " << argShortUsage;
			}
			std::cout << std::endl << std::endl;
			if(description != "") {
				std::cout << description << std::endl << std::endl;
			}
			option::printUsage(std::cout, usage);
		}
	
	/// \section Option Type Checks
	
		/// extended option argument type check class
		struct Arg : public option::Arg {

			static option::ArgStatus Unknown(const option::Option& option, bool msg) {
				if(msg) {
					std::cerr << "Unknown option \"" << option.name << "\"" << std::endl;
				}
				return option::ARG_ILLEGAL;
			}

			static option::ArgStatus Required(const option::Option& option, bool msg) {
				if(option.arg != 0) {
					return option::ARG_OK;
				}
				if(msg) {
					std::cerr << "Option \"" << option.name << "\" requires an argument" << std::endl;
				}
				return option::ARG_ILLEGAL;
			}

			static option::ArgStatus NonEmpty(const option::Option& option, bool msg) {
				if(option.arg != 0 && option.arg[0] != 0) {
					return option::ARG_OK;
				}
				if(msg) {
					std::cerr << "Option \"" << option.name << "\" requires a non-empty argument" << std::endl;
				}
				return option::ARG_ILLEGAL;
			}
			
			static option::ArgStatus Bool(const option::Option& option, bool msg) {
				if(option.arg != 0 && option.arg[0]  != 0) {
					std::string arg = option.arg;
					if(arg == "0" || arg == "1" || arg == "yes" || arg == "no" || arg == "true" || arg == "false") {
						return option::ARG_OK;
					}
				}
				if(msg) {
					std::cerr << "Option \"" << option.name << "\" requires a boolean argument" << std::endl;
				}
				return option::ARG_ILLEGAL;
			}

			static option::ArgStatus Integer(const option::Option& option, bool msg) {
				char *endptr = 0;
				if(option.arg != 0 && strtol(option.arg, &endptr, 0)){};
				if(endptr != option.arg && *endptr == 0) {
					return option::ARG_OK;
				}
				if(msg) {
					std::cerr << "Option \"" << option.name << "\" requires a non-decimal number argument" << std::endl;
				}
				return option::ARG_ILLEGAL;
			}
			
			static option::ArgStatus Decimal(const option::Option& option, bool msg) {
				char *endptr = 0;
				if(option.arg != 0 && strtod(option.arg, &endptr)){};
				if(endptr != option.arg && *endptr == 0) {
					return option::ARG_OK;
				}
				if(msg) {
					std::cerr << "Option \"" << option.name << "\" requires a number argument" << std::endl;
				}
				return option::ARG_ILLEGAL;
			}
		};
	
	protected:
	
		/// a non-option argument descriptor
		struct NonOption {
			const char *name; //< non-option argument name for printing
			const bool required; //< is this non-option argument required?
		};
	
		/// push descriptor struct, requires intermediate variable
		void pushDescriptor(const unsigned int index, const int type,
		                    const char* shortopt, const char *longopt,
		                    const option::CheckArg check, const char* help) {
			struct option::Descriptor d = {index, type, shortopt, longopt, check, help};
			descriptors.push_back(d);
		}
	
		/// push array terminator struct
		void pushEndDescriptor() {
			struct option::Descriptor end = {0, 0, NULL, NULL, 0, NULL};
			descriptors.push_back(end);
		}

		std::string description; //< optional description line
		std::string version; //< optional version string
		std::string name; //< program name (aka arg 0), only set after parsing

		std::map<std::string,unsigned int> indices; //< string keys to options index
		std::vector<option::Descriptor> descriptors; //< option descriptors
	
		option::Parser *parser; //< option parser, not vlai until after parsing
		option::Option *options; //< parsed options, arrayed by index
		option::Option *buffer; //< raw parsed option buffer
	
		std::vector<NonOption> nonOptions; //< non-option argument descriptors
		int numRequiredNonOptions; //< number of required non-option arguments
};
