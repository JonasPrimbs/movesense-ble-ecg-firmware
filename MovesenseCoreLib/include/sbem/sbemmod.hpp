#pragma once
#include <cstring>

#ifdef SBEM_DYNAMIC
#include <string>
#endif

#ifdef _MSC_VER
#define strdup _strdup
#endif

namespace nea
{
namespace sbem
{
/** Class for sbem descriptor modifier information.
 */
class Mod
{
public:
    /** Conversion type.
     */
    enum Type_e
    {
        Type_None,  /**< No conversion */
        Type_Float, /**< Conversion from/to float */
        Type_Custom /**< Custom conversion string, can't be automated by application */
    };

    /** Operator for conversion.
     */
    enum Operation_e
    {
        Operation_add,      /**< + */
        Operation_sub,      /**< - */
        Operation_multiply, /**< * */
        Operation_divide,   /**< / */
        Operation_none      /**< No operation, this can't be present with Type_Float */
    };

    /** Constructor.
     */
    Mod() : 
#ifndef SBEM_DYNAMIC
        m_custom(nullptr), 
#endif //ndef SBEM_DYNAMIC
        m_floats({0,0,0,0}), m_type(Type_None) {}

#ifdef SBEM_DYNAMIC
    /** Initialization.
     *
     * @param str Descriptor MOD string in Suunto Binary Encoded Message format.
     */
    void init(std::string& str)
    {
        float xval, yval;
        char xop, yop;

        m_type = Type_None;

        if (4 == sscanf(str.c_str(), "x%c%f,y%c%f", &xop, &xval, &yop, &yval))
        {
            switch (xop)
            {
            case '*':
                m_floats.m_xOp = Operation_multiply;
                break;
            case '/':
                m_floats.m_xOp = Operation_divide;
                break;
            case '-':
                m_floats.m_xOp = Operation_sub;
                break;
            case '+':
                m_floats.m_xOp = Operation_add;
                break;
            default:
                return;
            }
            switch (yop)
            {
            case '*':
                m_floats.m_yOp = Operation_multiply;
                break;
            case '/':
                m_floats.m_yOp = Operation_divide;
                break;
            case '-':
                m_floats.m_yOp = Operation_sub;
                break;
            case '+':
                m_floats.m_yOp = Operation_add;
                break;
            default:
                return;
            }
            m_floats.m_x = xval;
            m_floats.m_y = yval;
            m_type = Type_Float;
        }
        else
        {
            m_custom = strdup(str.c_str());
            if (m_custom.size())
            {
                m_type = Type_Custom;
            }
        }
    }
#endif

    /** Method for getting X operator in mod string
     *
     * @return Operator.
     */
    Operation_e xOperator() const
    {
        if (m_type == Type_Float)
        {
            return (Operation_e)m_floats.m_xOp;
        }
        return Operation_none;
    }

    /** Method for getting Y operator in mod string
     *
     * @return Operator.
     */
    Operation_e yOperator() const
    {
        if (m_type == Type_Float)
        {
            return (Operation_e)m_floats.m_yOp;
        }
        return Operation_none;
    }

    /** Method for getting conversion type
     * @return Conversion type
     */
    Type_e type() const { return (Type_e)m_type; }

    /** Method for getting X value.
     *
     * @return X
     */
    float x() const
    {
        if (m_type == Type_Float)
        {
            return m_floats.m_x;
        }
        return 0.0;
    }

    /** Method for getting Y value
     *
     * @return Y
     */
    float y() const
    {
        if (m_type == Type_Float)
        {
            return m_floats.m_y;
        }
        return 0.0;
    }

    /** Method for getting custom conversion string.
     *
     * @return custom conversion string.
     */
    char const* custom() const
    {
#ifdef SBEM_DYNAMIC
        return m_custom.c_str();
#else
        return m_custom;
#endif
    }

private:
    struct Floats
    {
        uint8_t m_xOp;
        uint8_t m_yOp;
        float m_x;
        float m_y;
    };

#ifdef SBEM_DYNAMIC
    std::string m_custom;
#else
    char* m_custom;
#endif
    uint8_t m_type;
    Floats m_floats;
};
}
}
