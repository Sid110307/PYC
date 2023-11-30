#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <stack>
#include <iomanip>

constexpr unsigned int PYC_MAGIC_NUMBER = 0xA0D0DA7;
constexpr int PYC_HEADER_SIZE = 16; // FIXME: This is wrong

enum Opcode : uint8_t
{
    CACHE = 0x0,
    POP_TOP = 0x1,
    PUSH_NULL = 0x2,
    NOP = 0x9,
    UNARY_POSITIVE = 0xA,
    UNARY_NEGATIVE = 0xB,
    UNARY_NOT = 0xC,
    UNARY_INVERT = 0xF,
    BINARY_SUBSCR = 0x19,
    GET_LEN = 0x1E,
    MATCH_MAPPING = 0x1F,
    MATCH_SEQUENCE = 0x20,
    MATCH_KEYS = 0x21,
    PUSH_EXC_INFO = 0x23,
    CHECK_EXC_MATCH = 0x24,
    CHECK_EG_MATCH = 0x25,
    WITH_EXCEPT_START = 0x31,
    GET_AITER = 0x32,
    GET_ANEXT = 0x33,
    BEFORE_ASYNC_WITH = 0x34,
    BEFORE_WITH = 0x35,
    END_ASYNC_FOR = 0x36,
    STORE_SUBSCR = 0x3C,
    DELETE_SUBSCR = 0x3D,
    GET_ITER = 0x44,
    GET_YIELD_FROM_ITER = 0x45,
    PRINT_EXPR = 0x46,
    LOAD_BUILD_CLASS = 0x47,
    LOAD_ASSERTION_ERROR = 0x4A,
    RETURN_GENERATOR = 0x4B,
    LIST_TO_TUPLE = 0x52,
    RETURN_VALUE = 0x53,
    IMPORT_STAR = 0x54,
    SETUP_ANNOTATIONS = 0x55,
    YIELD_VALUE = 0x56,
    ASYNC_GEN_WRAP = 0x57,
    PREP_RERAISE_STAR = 0x58,
    POP_EXCEPT = 0x59,
    STORE_NAME = 0x5A,
    DELETE_NAME = 0x5B,
    UNPACK_SEQUENCE = 0x5C,
    FOR_ITER = 0x5D,
    UNPACK_EX = 0x5E,
    STORE_ATTR = 0x5F,
    DELETE_ATTR = 0x60,
    STORE_GLOBAL = 0x61,
    DELETE_GLOBAL = 0x62,
    SWAP = 0x63,
    LOAD_CONST = 0x64,
    LOAD_NAME = 0x65,
    BUILD_TUPLE = 0x66,
    BUILD_LIST = 0x67,
    BUILD_SET = 0x68,
    BUILD_MAP = 0x69,
    LOAD_ATTR = 0x6A,
    COMPARE_OP = 0x6B,
    IMPORT_NAME = 0x6C,
    IMPORT_FROM = 0x6D,
    JUMP_FORWARD = 0x6E,
    JUMP_IF_FALSE_OR_POP = 0x6F,
    JUMP_IF_TRUE_OR_POP = 0x70,
    POP_JUMP_FORWARD_IF_FALSE = 0x72,
    POP_JUMP_FORWARD_IF_TRUE = 0x73,
    LOAD_GLOBAL = 0x74,
    IS_OP = 0x75,
    CONTAINS_OP = 0x76,
    RERAISE = 0x77,
    COPY = 0x78,
    BINARY_OP = 0x7A,
    SEND = 0x7B,
    LOAD_FAST = 0x7C,
    STORE_FAST = 0x7D,
    DELETE_FAST = 0x7E,
    POP_JUMP_FORWARD_IF_NOT_NONE = 0x80,
    POP_JUMP_FORWARD_IF_NONE = 0x81,
    RAISE_VARARGS = 0x82,
    GET_AWAITABLE = 0x83,
    MAKE_FUNCTION = 0x84,
    BUILD_SLICE = 0x85,
    JUMP_BACKWARD_NO_INTERRUPT = 0x86,
    MAKE_CELL = 0x87,
    LOAD_CLOSURE = 0x88,
    LOAD_DEREF = 0x89,
    STORE_DEREF = 0x8A,
    DELETE_DEREF = 0x8B,
    JUMP_BACKWARD = 0x8C,
    CALL_FUNCTION_EX = 0x8E,
    EXTENDED_ARG = 0x90,
    LIST_APPEND = 0x91,
    SET_ADD = 0x92,
    MAP_ADD = 0x93,
    LOAD_CLASSDEREF = 0x94,
    COPY_FREE_VARS = 0x95,
    RESUME = 0x97,
    MATCH_CLASS = 0x98,
    FORMAT_VALUE = 0x9B,
    BUILD_CONST_KEY_MAP = 0x9C,
    BUILD_STRING = 0x9D,
    LOAD_METHOD = 0xA0,
    LIST_EXTEND = 0xA2,
    SET_UPDATE = 0xA3,
    DICT_MERGE = 0xA4,
    DICT_UPDATE = 0xA5,
    PRECALL = 0xA6,
    CALL = 0xAB,
    KW_NAMES = 0xAC,
    POP_JUMP_BACKWARD_IF_NOT_NONE = 0xAD,
    POP_JUMP_BACKWARD_IF_NONE = 0xAE,
    POP_JUMP_BACKWARD_IF_FALSE = 0xAF,
    POP_JUMP_BACKWARD_IF_TRUE = 0xB0,
};

std::string opcodeToString(int opcode)
{
    switch (opcode)
    {
        case Opcode::CACHE:
            return "CACHE";
        case Opcode::POP_TOP:
            return "POP_TOP";
        case Opcode::PUSH_NULL:
            return "PUSH_NULL";
        case Opcode::NOP:
            return "NOP";
        case Opcode::UNARY_POSITIVE:
            return "UNARY_POSITIVE";
        case Opcode::UNARY_NEGATIVE:
            return "UNARY_NEGATIVE";
        case Opcode::UNARY_NOT:
            return "UNARY_NOT";
        case Opcode::UNARY_INVERT:
            return "UNARY_INVERT";
        case Opcode::BINARY_SUBSCR:
            return "BINARY_SUBSCR";
        case Opcode::GET_LEN:
            return "GET_LEN";
        case Opcode::MATCH_MAPPING:
            return "MATCH_MAPPING";
        case Opcode::MATCH_SEQUENCE:
            return "MATCH_SEQUENCE";
        case Opcode::MATCH_KEYS:
            return "MATCH_KEYS";
        case Opcode::PUSH_EXC_INFO:
            return "PUSH_EXC_INFO";
        case Opcode::CHECK_EXC_MATCH:
            return "CHECK_EXC_MATCH";
        case Opcode::CHECK_EG_MATCH:
            return "CHECK_EG_MATCH";
        case Opcode::WITH_EXCEPT_START:
            return "WITH_EXCEPT_START";
        case Opcode::GET_AITER:
            return "GET_AITER";
        case Opcode::GET_ANEXT:
            return "GET_ANEXT";
        case Opcode::BEFORE_ASYNC_WITH:
            return "BEFORE_ASYNC_WITH";
        case Opcode::BEFORE_WITH:
            return "BEFORE_WITH";
        case Opcode::END_ASYNC_FOR:
            return "END_ASYNC_FOR";
        case Opcode::STORE_SUBSCR:
            return "STORE_SUBSCR";
        case Opcode::DELETE_SUBSCR:
            return "DELETE_SUBSCR";
        case Opcode::GET_ITER:
            return "GET_ITER";
        case Opcode::GET_YIELD_FROM_ITER:
            return "GET_YIELD_FROM_ITER";
        case Opcode::PRINT_EXPR:
            return "PRINT_EXPR";
        case Opcode::LOAD_BUILD_CLASS:
            return "LOAD_BUILD_CLASS";
        case Opcode::LOAD_ASSERTION_ERROR:
            return "LOAD_ASSERTION_ERROR";
        case Opcode::RETURN_GENERATOR:
            return "RETURN_GENERATOR";
        case Opcode::LIST_TO_TUPLE:
            return "LIST_TO_TUPLE";
        case Opcode::RETURN_VALUE:
            return "RETURN_VALUE";
        case Opcode::IMPORT_STAR:
            return "IMPORT_STAR";
        case Opcode::SETUP_ANNOTATIONS:
            return "SETUP_ANNOTATIONS";
        case Opcode::YIELD_VALUE:
            return "YIELD_VALUE";
        case Opcode::ASYNC_GEN_WRAP:
            return "ASYNC_GEN_WRAP";
        case Opcode::PREP_RERAISE_STAR:
            return "PREP_RERAISE_STAR";
        case Opcode::POP_EXCEPT:
            return "POP_EXCEPT";
        case Opcode::STORE_NAME:
            return "STORE_NAME";
        case Opcode::DELETE_NAME:
            return "DELETE_NAME";
        case Opcode::UNPACK_SEQUENCE:
            return "UNPACK_SEQUENCE";
        case Opcode::FOR_ITER:
            return "FOR_ITER";
        case Opcode::UNPACK_EX:
            return "UNPACK_EX";
        case Opcode::STORE_ATTR:
            return "STORE_ATTR";
        case Opcode::DELETE_ATTR:
            return "DELETE_ATTR";
        case Opcode::STORE_GLOBAL:
            return "STORE_GLOBAL";
        case Opcode::DELETE_GLOBAL:
            return "DELETE_GLOBAL";
        case Opcode::SWAP:
            return "SWAP";
        case Opcode::LOAD_CONST:
            return "LOAD_CONST";
        case Opcode::LOAD_NAME:
            return "LOAD_NAME";
        case Opcode::BUILD_TUPLE:
            return "BUILD_TUPLE";
        case Opcode::BUILD_LIST:
            return "BUILD_LIST";
        case Opcode::BUILD_SET:
            return "BUILD_SET";
        case Opcode::BUILD_MAP:
            return "BUILD_MAP";
        case Opcode::LOAD_ATTR:
            return "LOAD_ATTR";
        case Opcode::COMPARE_OP:
            return "COMPARE_OP";
        case Opcode::IMPORT_NAME:
            return "IMPORT_NAME";
        case Opcode::IMPORT_FROM:
            return "IMPORT_FROM";
        case Opcode::JUMP_FORWARD:
            return "JUMP_FORWARD";
        case Opcode::JUMP_IF_FALSE_OR_POP:
            return "JUMP_IF_FALSE_OR_POP";
        case Opcode::JUMP_IF_TRUE_OR_POP:
            return "JUMP_IF_TRUE_OR_POP";
        case Opcode::POP_JUMP_FORWARD_IF_FALSE:
            return "POP_JUMP_FORWARD_IF_FALSE";
        case Opcode::POP_JUMP_FORWARD_IF_TRUE:
            return "POP_JUMP_FORWARD_IF_TRUE";
        case Opcode::LOAD_GLOBAL:
            return "LOAD_GLOBAL";
        case Opcode::IS_OP:
            return "IS_OP";
        case Opcode::CONTAINS_OP:
            return "CONTAINS_OP";
        case Opcode::RERAISE:
            return "RERAISE";
        case Opcode::COPY:
            return "COPY";
        case Opcode::BINARY_OP:
            return "BINARY_OP";
        case Opcode::SEND:
            return "SEND";
        case Opcode::LOAD_FAST:
            return "LOAD_FAST";
        case Opcode::STORE_FAST:
            return "STORE_FAST";
        case Opcode::DELETE_FAST:
            return "DELETE_FAST";
        case Opcode::POP_JUMP_FORWARD_IF_NOT_NONE:
            return "POP_JUMP_FORWARD_IF_NOT_NONE";
        case Opcode::POP_JUMP_FORWARD_IF_NONE:
            return "POP_JUMP_FORWARD_IF_NONE";
        case Opcode::RAISE_VARARGS:
            return "RAISE_VARARGS";
        case Opcode::GET_AWAITABLE:
            return "GET_AWAITABLE";
        case Opcode::MAKE_FUNCTION:
            return "MAKE_FUNCTION";
        case Opcode::BUILD_SLICE:
            return "BUILD_SLICE";
        case Opcode::JUMP_BACKWARD_NO_INTERRUPT:
            return "JUMP_BACKWARD_NO_INTERRUPT";
        case Opcode::MAKE_CELL:
            return "MAKE_CELL";
        case Opcode::LOAD_CLOSURE:
            return "LOAD_CLOSURE";
        case Opcode::LOAD_DEREF:
            return "LOAD_DEREF";
        case Opcode::STORE_DEREF:
            return "STORE_DEREF";
        case Opcode::DELETE_DEREF:
            return "DELETE_DEREF";
        case Opcode::JUMP_BACKWARD:
            return "JUMP_BACKWARD";
        case Opcode::CALL_FUNCTION_EX:
            return "CALL_FUNCTION_EX";
        case Opcode::EXTENDED_ARG:
            return "EXTENDED_ARG";
        case Opcode::LIST_APPEND:
            return "LIST_APPEND";
        case Opcode::SET_ADD:
            return "SET_ADD";
        case Opcode::MAP_ADD:
            return "MAP_ADD";
        case Opcode::LOAD_CLASSDEREF:
            return "LOAD_CLASSDEREF";
        case Opcode::COPY_FREE_VARS:
            return "COPY_FREE_VARS";
        case Opcode::RESUME:
            return "RESUME";
        case Opcode::MATCH_CLASS:
            return "MATCH_CLASS";
        case Opcode::FORMAT_VALUE:
            return "FORMAT_VALUE";
        case Opcode::BUILD_CONST_KEY_MAP:
            return "BUILD_CONST_KEY_MAP";
        case Opcode::BUILD_STRING:
            return "BUILD_STRING";
        case Opcode::LOAD_METHOD:
            return "LOAD_METHOD";
        case Opcode::LIST_EXTEND:
            return "LIST_EXTEND";
        case Opcode::SET_UPDATE:
            return "SET_UPDATE";
        case Opcode::DICT_MERGE:
            return "DICT_MERGE";
        case Opcode::DICT_UPDATE:
            return "DICT_UPDATE";
        case Opcode::PRECALL:
            return "PRECALL";
        case Opcode::CALL:
            return "CALL";
        case Opcode::KW_NAMES:
            return "KW_NAMES";
        case Opcode::POP_JUMP_BACKWARD_IF_NOT_NONE:
            return "POP_JUMP_BACKWARD_IF_NOT_NONE";
        case Opcode::POP_JUMP_BACKWARD_IF_NONE:
            return "POP_JUMP_BACKWARD_IF_NONE";
        case Opcode::POP_JUMP_BACKWARD_IF_FALSE:
            return "POP_JUMP_BACKWARD_IF_FALSE";
        case Opcode::POP_JUMP_BACKWARD_IF_TRUE:
            return "POP_JUMP_BACKWARD_IF_TRUE";
        default:
            return "UNKNOWN";
    }
}

std::vector<char> readFile(const std::string &filename)
{
    std::ifstream file(filename, std::ios::binary | std::ios::ate);
    if (!file.is_open()) throw std::runtime_error("Failed to open file: " + filename);

    std::streamsize size = file.tellg();
    file.seekg(0, std::ios::beg);

    std::vector<char> buffer(size);
    if (!file.read(buffer.data(), size)) throw std::runtime_error("Failed to read file: " + filename);

    return buffer;
}

void runCode(const std::vector<char> &bytecode)
{
    if ((int) bytecode.size() < PYC_HEADER_SIZE)
        throw std::runtime_error("Invalid bytecode size " + std::to_string(bytecode.size()) + ". Expected at least " +
                                 std::to_string(PYC_HEADER_SIZE) + ".");

    unsigned int magicNumber = *reinterpret_cast<const unsigned int *>(&bytecode[0]);
    if (magicNumber != PYC_MAGIC_NUMBER)
        throw std::runtime_error("Invalid magic number " + std::to_string(magicNumber) + ". Expected " +
                                 std::to_string(PYC_MAGIC_NUMBER) + ".");

    int codeObjectSize = *reinterpret_cast<const int *>(&bytecode[PYC_HEADER_SIZE]);
    std::vector<uint8_t> codeObject(bytecode.begin() + PYC_HEADER_SIZE + sizeof(int),
                                    bytecode.begin() + PYC_HEADER_SIZE + sizeof(int) + codeObjectSize);

    std::stack<int> stack;
    int instructionPointer = 0;

    int matchValue = 0, matchIndex = 0, matchCount = 0;
    std::vector<int> matchValues, matchIndices, matchCounts;

    int exception = 0, exceptionLine = 0, exceptionStackTop = 0;
    std::vector<int> exceptionStack;

    while (instructionPointer < (int) codeObject.size())
    {
        auto opcode = static_cast<Opcode>(codeObject[instructionPointer]);
        instructionPointer++;

        switch (opcode)
        {
            case Opcode::CACHE:
            case Opcode::NOP:
            {
                break;
            }
            case Opcode::POP_TOP:
            {
                if (stack.empty()) throw std::runtime_error("POP_TOP: Stack is empty.");

                stack.pop();
                break;
            }
            case Opcode::PUSH_NULL:
            {
                stack.push(0);
                break;
            }
            case Opcode::UNARY_POSITIVE:
            {
                if (stack.empty()) throw std::runtime_error("UNARY_POSITIVE: Stack is empty.");

                stack.top() = +stack.top();
                break;
            }
            case Opcode::UNARY_NEGATIVE:
            {
                if (stack.empty()) throw std::runtime_error("UNARY_NEGATIVE: Stack is empty.");

                stack.top() = -stack.top();
                break;
            }
            case Opcode::UNARY_NOT:
            {
                if (stack.empty()) throw std::runtime_error("UNARY_NOT: Stack is empty.");

                stack.top() = !stack.top();
                break;
            }
            case Opcode::UNARY_INVERT:
            {
                if (stack.empty()) throw std::runtime_error("UNARY_INVERT: Stack is empty.");

                stack.top() = ~stack.top();
                break;
            }
            case Opcode::BINARY_SUBSCR:
            {
                if (stack.size() < 2) throw std::runtime_error("BINARY_SUBSCR: Stack is empty.");

                stack.top() = stack.top() - stack.top();
                break;
            }
            case Opcode::GET_LEN:
            {
                if (stack.empty()) throw std::runtime_error("GET_LEN: Stack is empty.");

                int length = stack.top();
                stack.pop();

                stack.push(length);
                break;
            }
            case Opcode::MATCH_MAPPING:
            {
                if (stack.size() < 2) throw std::runtime_error("MATCH_MAPPING: Stack is empty.");

                matchValue = stack.top();
                stack.pop();

                matchIndex = stack.top();
                stack.pop();

                matchCount = stack.top();
                stack.pop();

                matchValues.resize(matchCount);
                matchIndices.resize(matchCount);
                matchCounts.resize(matchCount);

                for (int i = 0; i < matchCount; i++)
                {
                    matchValues[i] = stack.top();
                    stack.pop();

                    matchIndices[i] = stack.top();
                    stack.pop();

                    matchCounts[i] = stack.top();
                    stack.pop();
                }

                break;
            }
            case Opcode::MATCH_SEQUENCE:
            {
                if (stack.size() < 2) throw std::runtime_error("MATCH_SEQUENCE: Stack is empty.");

                matchValue = stack.top();
                stack.pop();

                matchIndex = stack.top();
                stack.pop();

                matchCount = stack.top();
                stack.pop();

                matchValues.resize(matchCount);
                matchIndices.resize(matchCount);
                matchCounts.resize(matchCount);

                for (int i = 0; i < matchCount; i++)
                {
                    matchValues[i] = stack.top();
                    stack.pop();

                    matchIndices[i] = stack.top();
                    stack.pop();

                    matchCounts[i] = stack.top();
                    stack.pop();
                }

                break;
            }
            case Opcode::MATCH_KEYS:
            {
                if (stack.size() < 2) throw std::runtime_error("MATCH_KEYS: Stack is empty.");

                matchValue = stack.top();
                stack.pop();

                matchIndex = stack.top();
                stack.pop();

                matchCount = stack.top();
                stack.pop();

                matchValues.resize(matchCount);
                matchIndices.resize(matchCount);
                matchCounts.resize(matchCount);

                for (int i = 0; i < matchCount; i++)
                {
                    matchValues[i] = stack.top();
                    stack.pop();

                    matchIndices[i] = stack.top();
                    stack.pop();

                    matchCounts[i] = stack.top();
                    stack.pop();
                }

                break;
            }
            case Opcode::PUSH_EXC_INFO:
            {
                if (stack.empty()) throw std::runtime_error("PUSH_EXC_INFO: Stack is empty.");

                exception = stack.top();
                stack.pop();

                exceptionLine = stack.top();
                stack.pop();

                exceptionStackTop = stack.top();
                stack.pop();

                exceptionStack.resize(exceptionStackTop);

                for (int i = 0; i < exceptionStackTop; i++)
                {
                    exceptionStack[i] = stack.top();
                    stack.pop();
                }

                stack.push(exceptionStackTop);
                stack.push(exceptionLine);
                stack.push(exception);
                break;
            }
            case Opcode::CHECK_EXC_MATCH:
            {
                if (stack.size() < 3) throw std::runtime_error("CHECK_EXC_MATCH: Stack is empty.");

                exception = stack.top();
                stack.pop();

                exceptionLine = stack.top();
                stack.pop();

                exceptionStackTop = stack.top();
                stack.pop();

                exceptionStack.resize(exceptionStackTop);

                for (int i = 0; i < exceptionStackTop; i++)
                {
                    exceptionStack[i] = stack.top();
                    stack.pop();
                }

                stack.push(exceptionStackTop);
                stack.push(exceptionLine);
                stack.push(exception);
                break;
            }
            case Opcode::CHECK_EG_MATCH:
            {
                if (stack.size() < 3) throw std::runtime_error("CHECK_EG_MATCH: Stack is empty.");

                exception = stack.top();
                stack.pop();

                exceptionLine = stack.top();
                stack.pop();

                exceptionStackTop = stack.top();
                stack.pop();

                exceptionStack.resize(exceptionStackTop);

                for (int i = 0; i < exceptionStackTop; i++)
                {
                    exceptionStack[i] = stack.top();
                    stack.pop();
                }

                stack.push(exceptionStackTop);
                stack.push(exceptionLine);
                stack.push(exception);
                break;
            }
            case Opcode::WITH_EXCEPT_START:
            {
                if (stack.size() < 4) throw std::runtime_error("WITH_EXCEPT_START: Stack is empty.");

                exception = stack.top();
                stack.pop();

                exceptionLine = stack.top();
                stack.pop();

                exceptionStackTop = stack.top();
                stack.pop();

                exceptionStack.resize(exceptionStackTop);

                for (int i = 0; i < exceptionStackTop; i++)
                {
                    exceptionStack[i] = stack.top();
                    stack.pop();
                }

                stack.push(exceptionStackTop);
                stack.push(exceptionLine);
                stack.push(exception);
                break;

            }
            case Opcode::GET_AITER:
            case Opcode::GET_ANEXT:
            case Opcode::BEFORE_ASYNC_WITH:
            case Opcode::BEFORE_WITH:
            case Opcode::END_ASYNC_FOR:
            case Opcode::STORE_SUBSCR:
            case Opcode::DELETE_SUBSCR:
            case Opcode::GET_ITER:
            case Opcode::GET_YIELD_FROM_ITER:
            case Opcode::PRINT_EXPR:
            case Opcode::LOAD_BUILD_CLASS:
            case Opcode::LOAD_ASSERTION_ERROR:
            case Opcode::RETURN_GENERATOR:
            case Opcode::LIST_TO_TUPLE:
            case Opcode::RETURN_VALUE:
            case Opcode::IMPORT_STAR:
            case Opcode::SETUP_ANNOTATIONS:
            case Opcode::YIELD_VALUE:
            case Opcode::ASYNC_GEN_WRAP:
            case Opcode::PREP_RERAISE_STAR:
            case Opcode::POP_EXCEPT:
            case Opcode::STORE_NAME:
            case Opcode::DELETE_NAME:
            case Opcode::UNPACK_SEQUENCE:
            case Opcode::FOR_ITER:
            case Opcode::UNPACK_EX:
            case Opcode::STORE_ATTR:
            case Opcode::DELETE_ATTR:
            case Opcode::STORE_GLOBAL:
            case Opcode::DELETE_GLOBAL:
            case Opcode::SWAP:
            case Opcode::LOAD_CONST:
            case Opcode::LOAD_NAME:
            case Opcode::BUILD_TUPLE:
            case Opcode::BUILD_LIST:
            case Opcode::BUILD_SET:
            case Opcode::BUILD_MAP:
            case Opcode::LOAD_ATTR:
            case Opcode::COMPARE_OP:
            case Opcode::IMPORT_NAME:
            case Opcode::IMPORT_FROM:
            case Opcode::JUMP_FORWARD:
            case Opcode::JUMP_IF_FALSE_OR_POP:
            case Opcode::JUMP_IF_TRUE_OR_POP:
            case Opcode::POP_JUMP_FORWARD_IF_FALSE:
            case Opcode::POP_JUMP_FORWARD_IF_TRUE:
            case Opcode::LOAD_GLOBAL:
            case Opcode::IS_OP:
            case Opcode::CONTAINS_OP:
            case Opcode::RERAISE:
            case Opcode::COPY:
            case Opcode::BINARY_OP:
            case Opcode::SEND:
            case Opcode::LOAD_FAST:
            case Opcode::STORE_FAST:
            case Opcode::DELETE_FAST:
            case Opcode::POP_JUMP_FORWARD_IF_NOT_NONE:
            case Opcode::POP_JUMP_FORWARD_IF_NONE:
            case Opcode::RAISE_VARARGS:
            case Opcode::GET_AWAITABLE:
            case Opcode::MAKE_FUNCTION:
            case Opcode::BUILD_SLICE:
            case Opcode::JUMP_BACKWARD_NO_INTERRUPT:
            case Opcode::MAKE_CELL:
            case Opcode::LOAD_CLOSURE:
            case Opcode::LOAD_DEREF:
            case Opcode::STORE_DEREF:
            case Opcode::DELETE_DEREF:
            case Opcode::JUMP_BACKWARD:
            case Opcode::CALL_FUNCTION_EX:
            case Opcode::EXTENDED_ARG:
            case Opcode::LIST_APPEND:
            case Opcode::SET_ADD:
            case Opcode::MAP_ADD:
            case Opcode::LOAD_CLASSDEREF:
            case Opcode::COPY_FREE_VARS:
            case Opcode::RESUME:
            case Opcode::MATCH_CLASS:
            case Opcode::FORMAT_VALUE:
            case Opcode::BUILD_CONST_KEY_MAP:
            case Opcode::BUILD_STRING:
            case Opcode::LOAD_METHOD:
            case Opcode::LIST_EXTEND:
            case Opcode::SET_UPDATE:
            case Opcode::DICT_MERGE:
            case Opcode::DICT_UPDATE:
            case Opcode::PRECALL:
            case Opcode::CALL:
            case Opcode::KW_NAMES:
            case Opcode::POP_JUMP_BACKWARD_IF_NOT_NONE:
            case Opcode::POP_JUMP_BACKWARD_IF_NONE:
            case Opcode::POP_JUMP_BACKWARD_IF_FALSE:
            case Opcode::POP_JUMP_BACKWARD_IF_TRUE:
            default:
                throw std::runtime_error(
                        "Unknown opcode 0x" + std::to_string(opcode) + " (" + opcodeToString(opcode) + ").");
        }
    }
}

int main(int argc, char **argv)
{
    if (argc < 2)
    {
        std::cerr << "Usage: " << argv[0] << " <file.pyc>" << std::endl;
        std::cout << "Note: Only Python 3.8+ is supported." << std::endl;

        return EXIT_FAILURE;
    }

    try
    {
        runCode(readFile(argv[1]));
    }
    catch (const std::exception &ex)
    {
        std::cerr << "Error: " << ex.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
