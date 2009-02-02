/*
 * opencog/atomspace/Node.cc
 *
 * Copyright (C) 2002-2007 Novamente LLC
 * All Rights Reserved
 *
 * Written by Thiago Maia <thiago@vettatech.com>
 *            Andre Senna <senna@vettalabs.com>
 *            Welter Silva <welter@vettalabs.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License v3 as
 * published by the Free Software Foundation and including the exceptions
 * at http://opencog.org/wiki/Licenses
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program; if not, write to:
 * Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#include "Node.h"

#include <stdio.h>

#include <opencog/atomspace/AtomSpaceDefinitions.h>
#include <opencog/atomspace/ClassServer.h>
#include <opencog/atomspace/Link.h>
#include <opencog/atomspace/TLB.h>
#include <opencog/util/Logger.h>
#include <opencog/util/platform.h>

using namespace opencog;

Node::Node(Type type,
           const std::string& cname,
           const TruthValue& tv)
throw (InvalidParamException, AssertionException)
#ifndef PUT_OUTGOING_SET_IN_LINKS
        : Atom(type, std::vector<Handle>(), tv)
#else
        : Atom(type, tv)
#endif
{
    if (!ClassServer::isAssignableFrom(NODE, type)) {
        throw InvalidParamException(TRACE_INFO, "Node - Invalid node type '%d'.", type);
    }
    name = cname;
}

Node::~Node() throw ()
{
//    fprintf(stdout, "Deleting Node:\n%s\n", toString().c_str());
//    fflush(stdout);
}

const std::string& Node::getName() const
{
    return name;
}

void Node::setName(const std::string& cname) throw (RuntimeException)
{
    if (atomTable != NULL) {
        throw RuntimeException(TRACE_INFO,
            "Node - Cannot change the name of a node already "
            "inserted into an AtomTable.");
    }
    name = cname;
}

void Node::merge(Atom* atom) throw (InconsistenceException)
{
    if (*this != *atom)
        throw InconsistenceException(TRACE_INFO,
             "Node - Different nodes cannot be merged");
    Atom::merge(atom);
}

std::string Node::toShortString() const
{
#define BUFSZ 1024
    char buf[BUFSZ];
    std::string tmpname = name;
    if (name == "")
        tmpname = "#" + TLB::getHandle(this).str();
    snprintf(buf, BUFSZ, "node[%s:%s%s]",
             ClassServer::getTypeName(type).c_str(), tmpname.c_str(),
                    (getFlag(HYPOTETHICAL_FLAG) ? ":h" : ""));
    return buf;
}

std::string Node::toString() const
{
    char buf[BUFSZ];
    std::string tmpname = name;
    if (name == "")
        tmpname = "#" + TLB::getHandle(this).str();
    //activation here at 0: can be replace with LTI
    snprintf(buf, BUFSZ, "node[%s:%s] av:(%d,%d) tv:(%f,%f)",
             ClassServer::getTypeName(type).c_str(), tmpname.c_str(),
             (int)getAttentionValue().getSTI(),
             (int)getAttentionValue().getLTI(),
             getTruthValue().getMean(), getTruthValue().getConfidence());
    return buf;
}

bool Node::operator==(const Atom& other) const
{
    return (getType() == other.getType()) &&
           (getName() == dynamic_cast<const Node&>(other).getName());
}

bool Node::operator!=(const Atom& other) const
{
    return !(*this == other);
}

size_t Node::hashCode() const
{
    return getType() ^ hash<std::string>()(name);
}
