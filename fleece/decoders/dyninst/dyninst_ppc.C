
/*
 * See peach/COPYRIGHT for copyright information.
 *
 * This file is a part of Fleece.
 *
 * Fleece is free software; you can redistribute it and/or modify it under the
 * terms of the GNU Lesser General Public License as published by the Free
 * Software Foundation; either version 3.0 of the License, or (at your option)
 * any later version.
 *  
 * This software is distributed in the hope that it will be useful, but WITHOUT 
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more
 * details.
 * 
 * You should have received a copy of the GNU Lesser General Public License
 * along with this software; if not, see www.gnu.org/licenses
*/

#include "Decoder.h"
#include "Normalization.h"
#include "InstructionDecoder.h"
#include "StringUtils.h"

using namespace Dyninst;
using namespace InstructionAPI;

void removeImplicitLI0x0(char* buf, int bufLen) {
    if (strncmp(buf, "li", 2)) {
        return;
    }
    char* cur = buf;
    while (*cur && *cur != ',') {
        ++cur;
    }
    if (!*cur) {
        return;
    }
    if (!strncmp(cur, ", 0x0", 5)) {
        strncpy(cur, cur + 5, bufLen - (cur - buf) - 5);
    }
}

FindList* initSwapPowerPCOperandsFindList() {
    FindList* fl = new FindList(877);
    Normalization::addOperandSwapTerm(*fl, "eqv", 1, 2);
    Normalization::addOperandSwapTerm(*fl, "orc ", 1, 2);
    Normalization::addOperandSwapTerm(*fl, "orc. ", 1, 2);
    Normalization::addOperandSwapTerm(*fl, "or ", 1, 2);
    Normalization::addOperandSwapTerm(*fl, "or. ", 1, 2);
    Normalization::addOperandSwapTerm(*fl, "and ", 1, 2);
    Normalization::addOperandSwapTerm(*fl, "and. ", 1, 2);
    Normalization::addOperandSwapTerm(*fl, "srd", 1, 2);
    Normalization::addOperandSwapTerm(*fl, "sld", 1, 2);
    Normalization::addOperandSwapTerm(*fl, "slw", 1, 2);
    Normalization::addOperandSwapTerm(*fl, "sraw", 1, 2);
    Normalization::addOperandSwapTerm(*fl, "srad", 1, 2);
    Normalization::addOperandSwapTerm(*fl, "srw", 1, 2);
    Normalization::addOperandSwapTerm(*fl, "rlwimi", 1, 2);
    Normalization::addOperandSwapTerm(*fl, "fnmadd", 3, 4);
    Normalization::addOperandSwapTerm(*fl, "fmadd", 3, 4);
    Normalization::addOperandSwapTerm(*fl, "fnmsub", 3, 4);
    Normalization::addOperandSwapTerm(*fl, "fmsub", 3, 4);
    Normalization::addOperandSwapTerm(*fl, "fsel", 3, 4);
    Normalization::addOperandSwapTerm(*fl, "cntlz", 1, 2);
    Normalization::addOperandSwapTerm(*fl, "extsb", 1, 2);
    Normalization::addOperandSwapTerm(*fl, "extsw", 1, 2);
    Normalization::addOperandSwapTerm(*fl, "sthcx", 2, 3);
    Normalization::addOperandSwapTerm(*fl, "stwcx", 2, 3);
    Normalization::addOperandSwapTerm(*fl, "stbcx", 2, 3);
    Normalization::addOperandSwapTerm(*fl, "stdcx", 2, 3);
    Normalization::addOperandSwapTerm(*fl, "sthx", 2, 3);
    Normalization::addOperandSwapTerm(*fl, "sthux", 2, 3);
    Normalization::addOperandSwapTerm(*fl, "sthbrx", 2, 3);
    Normalization::addOperandSwapTerm(*fl, "stwbrx", 2, 3);
    Normalization::addOperandSwapTerm(*fl, "stbbrx", 2, 3);
    Normalization::addOperandSwapTerm(*fl, "stdbrx", 2, 3);
    Normalization::addOperandSwapTerm(*fl, "stdx", 2, 3);
    Normalization::addOperandSwapTerm(*fl, "stdux", 2, 3);
    Normalization::addOperandSwapTerm(*fl, "stbx", 2, 3);
    Normalization::addOperandSwapTerm(*fl, "stbux", 2, 3);
    Normalization::addOperandSwapTerm(*fl, "stwx", 2, 3);
    Normalization::addOperandSwapTerm(*fl, "stwux", 2, 3);
    Normalization::addOperandSwapTerm(*fl, "stfsx", 2, 3);
    Normalization::addOperandSwapTerm(*fl, "stfdx", 2, 3);
    Normalization::addOperandSwapTerm(*fl, "stfsux", 2, 3);
    Normalization::addOperandSwapTerm(*fl, "stfdux", 2, 3);
    Normalization::addOperandSwapTerm(*fl, "stfiwx", 2, 3);
    Normalization::addOperandSwapTerm(*fl, "lfdux", 2, 3);
    Normalization::addOperandSwapTerm(*fl, "lfsux", 2, 3);
    Normalization::addOperandSwapTerm(*fl, "lfdx", 2, 3);
    Normalization::addOperandSwapTerm(*fl, "lfsx", 2, 3);
    Normalization::addOperandSwapTerm(*fl, "lbzx", 2, 3);
    Normalization::addOperandSwapTerm(*fl, "lhzx", 2, 3);
    Normalization::addOperandSwapTerm(*fl, "lwzx", 2, 3);
    Normalization::addOperandSwapTerm(*fl, "ldzx", 2, 3);
    Normalization::addOperandSwapTerm(*fl, "lbx", 2, 3);
    Normalization::addOperandSwapTerm(*fl, "lhx", 2, 3);
    Normalization::addOperandSwapTerm(*fl, "lwx", 2, 3);
    Normalization::addOperandSwapTerm(*fl, "ldx", 2, 3);
    Normalization::addOperandSwapTerm(*fl, "lbax", 2, 3);
    Normalization::addOperandSwapTerm(*fl, "lhax", 2, 3);
    Normalization::addOperandSwapTerm(*fl, "lwax", 2, 3);
    Normalization::addOperandSwapTerm(*fl, "ldax", 2, 3);
    Normalization::addOperandSwapTerm(*fl, "lbzux", 2, 3);
    Normalization::addOperandSwapTerm(*fl, "lhzux", 2, 3);
    Normalization::addOperandSwapTerm(*fl, "lwzux", 2, 3);
    Normalization::addOperandSwapTerm(*fl, "ldzux", 2, 3);
    Normalization::addOperandSwapTerm(*fl, "lbux", 2, 3);
    Normalization::addOperandSwapTerm(*fl, "lhux", 2, 3);
    Normalization::addOperandSwapTerm(*fl, "lwux", 2, 3);
    Normalization::addOperandSwapTerm(*fl, "ldux", 2, 3);
    Normalization::addOperandSwapTerm(*fl, "lbrx", 2, 3);
    Normalization::addOperandSwapTerm(*fl, "lhrx", 2, 3);
    Normalization::addOperandSwapTerm(*fl, "lwrx", 2, 3);
    Normalization::addOperandSwapTerm(*fl, "ldrx", 2, 3);
    Normalization::addOperandSwapTerm(*fl, "lbbrx", 2, 3);
    Normalization::addOperandSwapTerm(*fl, "lhbrx", 2, 3);
    Normalization::addOperandSwapTerm(*fl, "lwbrx", 2, 3);
    Normalization::addOperandSwapTerm(*fl, "ldbrx", 2, 3);
    Normalization::addOperandSwapTerm(*fl, "lbarx", 2, 3);
    Normalization::addOperandSwapTerm(*fl, "lharx", 2, 3);
    Normalization::addOperandSwapTerm(*fl, "lwarx", 2, 3);
    Normalization::addOperandSwapTerm(*fl, "ldarx", 2, 3);
    return fl;
}

void swapPowerPCOperands(char* buf, int bufLen) {
    static FindList* fl = initSwapPowerPCOperandsFindList();
    if (strncmp(buf, "cr", 2)) {
        fl->process(buf, bufLen);
    }
}

void signedPPCDisplacements(char* buf, int bufLen) {
    char* cur = buf;
    char* place;
    while (*cur) {
        if (*cur == '0' && *(cur + 1) == 'x') {
            place = cur;
            cur += 2;
            while (isxdigit(*cur)) {
                ++cur;
            }
            short disp = (short)strtol(place, NULL, 16);
            if ((*cur == '(' && (cur == place + 6 && disp < 0)) ||
                (!*cur && (!strncmp(buf, "tdi", 3) || 
                           !strncmp(buf, "twi", 3) || 
                           !strncmp(buf, "li ", 3) || 
                           !strncmp(buf, "cmpi", 4) || 
                           !strncmp(buf, "mulli", 5) || 
                           !strncmp(buf, "addi", 4) || 
                           !strncmp(buf, "subfic", 6)))) {
                disp = disp * -1;
                char temp[bufLen - (cur - buf)];
                strcpy(temp, cur);
                snprintf(place, bufLen - (place - buf), "-0x%x%s", (int)disp, temp);
            }
        }
        ++cur;
    }
}


int dyninst_ppc_decode(char* inst, int nBytes, char* buf, int bufLen) {

    if (nBytes == 0) {
        return -1;
    }

    InstructionDecoder d(inst, nBytes, Arch_ppc64);
    Instruction::Ptr p = d.decode();
    strncpy(buf, p->format().c_str(), bufLen);

    return 0;
}

void dyninst_ppc_norm(char* buf, int bufLen) {
    removeCharacter(buf, bufLen, '%');
    removeImplicitLI0x0(buf, bufLen);
    signedPPCDisplacements(buf, bufLen);
    swapPowerPCOperands(buf, bufLen);
}
Decoder* dec_dyninst_ppc = new Decoder(&dyninst_ppc_decode, 
            NULL, &dyninst_ppc_norm, "dyninst", "ppc");
