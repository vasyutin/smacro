// -*- Mode: c++; c-basic-offset: 4; tab-width: 4; -*-

/******************************************************************************
 *
 *  file:  ValuesConstraint.h
 *
 *  Copyright (c) 2005, Michael E. Smoot
 *  Copyright (c) 2017, Google LLC
 *  Copyright (c) 2022 Sergey Vasyutin .
 *  All rights reserved.
 *
 *  See the file COPYING in the top directory of this distribution for
 *  more information.
 *
 *  THE SOFTWARE IS PROVIDED _AS IS_, WITHOUT WARRANTY OF ANY KIND, EXPRESS
 *  OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
 *  THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 *  FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 *  DEALINGS IN THE SOFTWARE.
 *
 *****************************************************************************/

#ifndef TCLAP_VALUES_CONSTRAINT_H
#define TCLAP_VALUES_CONSTRAINT_H

#include <wtclap/Constraint.h>
#include <wtclap/sstream.h>

#include <string>
#include <vector>

namespace TCLAP {

/**
 * A Constraint that constrains the Arg to only those values specified
 * in the constraint.
 */
template <class T>
class ValuesConstraint : public Constraint<T> {
public:
    /**
     * Constructor.
     * \param allowed - vector of allowed values.
     */
    explicit ValuesConstraint(std::vector<T> &allowed);

    /**
     * Virtual destructor.
     */
    virtual ~ValuesConstraint() {}

    /**
     * Returns a description of the Constraint.
     */
    virtual std::wstring description() const;

    /**
     * Returns the short ID for the Constraint.
     */
    virtual std::wstring shortID() const;

    /**
     * The method used to verify that the value parsed from the command
     * line meets the constraint.
     * \param value - The value that will be checked.
     */
    virtual bool check(const T &value) const;

protected:
    /**
     * The list of valid values.
     */
    std::vector<T> _allowed;

    /**
     * The string used to describe the allowed values of this constraint.
     */
    std::wstring _typeDesc;
};

template <class T>
ValuesConstraint<T>::ValuesConstraint(std::vector<T> &allowed)
    : _allowed(allowed), _typeDesc(L"") {
    for (unsigned int i = 0; i < _allowed.size(); i++) {
        std::wostringstream os;
        os << _allowed[i];

        std::wstring temp(os.str());

        if (i > 0) _typeDesc += L"|";
        _typeDesc += temp;
    }
}

template <class T>
bool ValuesConstraint<T>::check(const T &val) const {
    if (std::find(_allowed.begin(), _allowed.end(), val) == _allowed.end())
        return false;
    else
        return true;
}

template <class T>
std::wstring ValuesConstraint<T>::shortID() const {
    return _typeDesc;
}

template <class T>
std::wstring ValuesConstraint<T>::description() const {
    return _typeDesc;
}

}  // namespace TCLAP
#endif  // TCLAP_VALUES_CONSTRAINT_H
