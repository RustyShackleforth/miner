/*
 * Copyright (C) 2002-2007 Novamente LLC
 * Copyright (C) 2008 by Singularity Institute for Artificial Intelligence
 * All Rights Reserved
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

#ifndef LINK2LINKRULE_H
#define LINK2LINKRULE_H

namespace reasoning
{

extern std::map<int, string> type2name;
Rule::setOfMPs makeSingletonSet(Rule::MPs& mp);

//template<typename FormulaType, Type SRC_LINK, Type DEST_LINK>
template<typename FormulaType>
class Link2LinkRule : public GenericRule<FormulaType>
{
    Type SRC_LINK;
    Type DEST_LINK;
protected:
//	mutable std::vector<Type> ti;

public:
	bool validate2				(Rule::MPs& args) const { return true; }
	//Link2LinkRule(iAtomSpaceWrapper *_destTable)
	//: GenericRule<FormulaType>(_destTable,false,"")
	Link2LinkRule(iAtomSpaceWrapper *_destTable, Type src, Type dest)
	: GenericRule<FormulaType>(_destTable,false,""), SRC_LINK(src), DEST_LINK(dest)
	{
		GenericRule<FormulaType>::name = "Link2Link(" + type2name[SRC_LINK] + "=>" + type2name[DEST_LINK] +")";
		GenericRule<FormulaType>::inputFilter.push_back(meta(
			new tree<Vertex>(
				mva((Handle)SRC_LINK,
					mva((Handle)ATOM),
					mva((Handle)ATOM))
			)));
	}
	Rule::setOfMPs o2iMetaExtra(meta outh, bool& overrideInputFilter) const
	{
		if (!GET_ATW->inheritsType((Type)(int)boost::get<Handle>(*outh->begin()).value(), DEST_LINK))
			return Rule::setOfMPs();

		Rule::MPs ret;

		BBvtree ret_m(new BoundVTree(*outh));
		*ret_m->begin() = Vertex((Handle)SRC_LINK);
		ret.push_back(ret_m);
		
		overrideInputFilter = true;

		return makeSingletonSet(ret);
	}

	virtual TruthValue** formatTVarray(const vector<Vertex>& premiseArray, int* newN) const
	{
		TruthValue** tvs = (TruthValue**)new SimpleTruthValue*[1];

		assert(premiseArray.size()==1);

		tvs[0] = (TruthValue*) &(GET_ATW->getTV(boost::get<Handle>(premiseArray[0])));

		return tvs;
	}

	virtual meta i2oType(const vector<Vertex>& h) const
	{
		assert(1==h.size());

		meta ret = atomWithNewType(h[0], DEST_LINK);
        cprintf(3,"i2otype() outputs: ");
#if 0
        rawPrint(*ret, ret->begin(), 3);
#else
        NMPrinter printer(NMP_HANDLE|NMP_TYPE_NAME, 
                      NM_PRINTER_DEFAULT_TRUTH_VALUE_PRECISION, 
                      NM_PRINTER_DEFAULT_INDENTATION_TAB_SIZE,
					  3);
        printer.print(ret->begin());
#endif

		return ret;
	}
	NO_DIRECT_PRODUCTION;
};

/*
typedef  Link2LinkRule<Mem2InhFormula, MEMBER_LINK, EXTENSIONAL_INHERITANCE_LINK> Mem2InhRule;
typedef  Link2LinkRule<Int2ExtFormula, IMPLICATION_LINK, MIXED_IMPLICATION_LINK>				IntImp2ExtRule;
//typedef  Link2LinkRule<Int2ExtFormula, INHERITANCE_LINK, MIXED_INHERITANCE_LINK>				IntInh2ExtRule;
typedef  Link2LinkRule<Int2ExtFormula, INHERITANCE_LINK, EXTENSIONAL_INHERITANCE_LINK>				IntInh2ExtRule;
typedef  Link2LinkRule<Ext2IntFormula, EXTENSIONAL_IMPLICATION_LINK, MIXED_IMPLICATION_LINK> ExtImp2IntRule;
typedef  Link2LinkRule<Ext2IntFormula, EXTENSIONAL_INHERITANCE_LINK, INHERITANCE_LINK> ExtInh2IntRule;
//typedef  Link2LinkRule<Ext2IntFormula, EXTENSIONAL_INHERITANCE_LINK, MIXED_INHERITANCE_LINK> ExtInh2IntRule;
typedef  Link2LinkRule<Inh2ImpFormula, INHERITANCE_LINK, IMPLICATION_LINK>					Inh2ImpRule;
typedef  Link2LinkRule<Imp2InhFormula, IMPLICATION_LINK, INHERITANCE_LINK>					Imp2InhRule;
typedef  Link2LinkRule<Mem2EvalFormula, MEMBER_LINK, EVALUATION_LINK> Mem2EvalRule;
//typedef  Link2LinkRule<Eval2InhFormula, EVALUATION_LINK, INHERITANCE_LINK> Eval2InhRule;
//typedef  Link2LinkRule<Formula, _LINK, _LINK> Rule;
*/

typedef  Link2LinkRule<Mem2InhFormula> Mem2InhRule;
typedef  Link2LinkRule<Int2ExtFormula> Int2ExtRule;
typedef  Link2LinkRule<Ext2IntFormula> Ext2IntRule;
typedef  Link2LinkRule<Inh2ImpFormula> Inh2ImpRule;
typedef  Link2LinkRule<Imp2InhFormula> Imp2InhRule;
typedef  Link2LinkRule<Mem2EvalFormula> Mem2EvalRule;

} // namespace reasoning
#endif // LINK2LINKRULE_H
