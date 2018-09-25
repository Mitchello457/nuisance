# Copyright 2016 L. Pickering, P Stowell, R. Terri, C. Wilkinson, C. Wret

################################################################################
#    This file is part of NUISANCE.
#
#    NUISANCE is free software: you can redistribute it and/or modify
#    it under the terms of the GNU General Public License as published by
#    the Free Software Foundation, either version 3 of the License, or
#    (at your option) any later version.
#
#    NUISANCE is distributed in the hope that it will be useful,
#    but WITHOUT ANY WARRANTY; without even the implied warranty of
#    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#    GNU General Public License for more details.
#
#    You should have received a copy of the GNU General Public License
#    along with NUISANCE.  If not, see <http://www.gnu.org/licenses/>.
################################################################################

if(NEUT_ROOT STREQUAL "")
  cmessage(FATAL_ERROR "Variable NEUT_ROOT is not defined. Please export environment variable NEUT_ROOT or configure with -DNEUT_ROOT=/path/to/NEUT. This must be set to point to a prebuilt NEUT instance.")
endif()

if(CERN STREQUAL "")
  cmessage(FATAL_ERROR "Variable CERN is not defined. Please export environment variable CERN or configure with -DCERN=/path/to/CERNLIB. This must be set to point to a prebuilt CERNLIB instance.")
endif()

if(CERN_LEVEL STREQUAL "")
    cmessage(FATAL_ERROR "Variable CERN_LEVEL is not defined. Please export environment variable CERN_LEVEL or configure with -DCERN_LEVEL=XXXX (likely to be 2005).")
endif()

if(NOT IS_NEUT_54)
  set(NEUT_LIB_DIR ${NEUT_ROOT}/lib/Linux_pc)
else()
  set(NEUT_LIB_DIR ${NEUT_ROOT}/lib)
endif()
set(NEUT_CLASS ${NEUT_ROOT}/src/neutclass)

LIST(APPEND EXTRA_CXX_FLAGS -D__NEUT_ENABLED__ )

LIST(APPEND RWENGINE_INCLUDE_DIRECTORIES
  ${NEUT_ROOT}/include
  ${NEUT_ROOT}/src/neutclass
  ${NEUT_ROOT}/src/reweight)

LIST(APPEND EXTRA_LINK_DIRS
  ${NEUT_LIB_DIR}
  ${CERN}/${CERN_LEVEL}/lib
  ${NEUT_ROOT}/src/reweight)

if(NOT IS_NEUT_54)
  LIST(APPEND EXTRA_LIBS
    NReWeight
    neutcore
    nuccorrspl
    nuceff
    partnuck
    skmcsvc
    tauola
    jetset74
    pdflib804
    mathlib
    packlib
    pawlib)
else()
  LIST(APPEND EXTRA_LIBS
    NReWeight
    neutcore_5.4.0
    nuccorspl_5.4.0 #typo in NEUT, may hopefully disappear
    nuceff_5.4.0
    partnuck_5.4.0
    skmcsvc_5.4.0
    tauola_5.4.0
    HT2p2h_5.4.0
    N1p1h_5.4.0
    jetset74
    pdflib804
    mathlib
    packlib
    pawlib)
endif()

set(NEUT_ROOT_LIBS)

LIST(APPEND NEUT_ROOT_LIBS
  ${NEUT_CLASS}/neutctrl.so
  ${NEUT_CLASS}/neutfsivert.so)

# Check for new versions of NEUT with NUCLEON FSI
if(EXISTS "${NEUT_CLASS}/neutnucfsistep.so")
  set(NEUT_NUCFSI 1)
  LIST(APPEND EXTRA_CXX_FLAGS -D__NEUT_NUCFSI_ENABLED__ )

  LIST(APPEND NEUT_ROOT_LIBS
    ${NEUT_CLASS}/neutnucfsistep.so
    ${NEUT_CLASS}/neutnucfsivert.so
    )
endif()

if(NOT IS_NEUT_54)
  LIST(APPEND NEUT_ROOT_LIBS
    ${NEUT_CLASS}/neutrootTreeSingleton.so)
endif()

LIST(APPEND NEUT_ROOT_LIBS
  ${NEUT_CLASS}/neutvtx.so
  ${NEUT_CLASS}/neutfsipart.so
  ${NEUT_CLASS}/neutpart.so
  ${NEUT_CLASS}/neutvect.so
  )

foreach(OBJ ${NEUT_ROOT_LIBS})
  LIST(APPEND EXTRA_SHAREDOBJS ${OBJ})
endforeach()
