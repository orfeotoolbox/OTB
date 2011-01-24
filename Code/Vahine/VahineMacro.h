/*
 * VahineMacro.h
 *
 * Vahine Framework
 * Copyright (C) 2008, 2009 Ludovic Léau-Mercier and Laboratoire de Planétologie de Grenoble
 * See LICENCE and COPYING for details.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * CeCILLl License for more details and http://www.cecill.info
 *
 */
#ifndef __VAHINE_MACRO_H
#define __VAHINE_MACRO_H

#define OTB_SHOW_ALL_MSG_DEBUG
#include <otbMacro.h>

#define vahineDebug(x) {otbDebugMacro(<<"[VAHINE] "<< x <<std::endl) }
#define vahineDebugNel(x) {otbDebugMacro(<<"[VAHINE] "<< x) }

#define vahineWarning(x) {itkWarningMacro(<<"[VAHINE] "<< x )}

#endif /* __VAHINE_MACRO_H */
