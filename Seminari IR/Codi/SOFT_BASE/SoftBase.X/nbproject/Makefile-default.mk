#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Include project Makefile
ifeq "${IGNORE_LOCAL}" "TRUE"
# do not include local makefile. User is passing all local related variables already
else
include Makefile
# Include makefile containing local settings
ifeq "$(wildcard nbproject/Makefile-local-default.mk)" "nbproject/Makefile-local-default.mk"
include nbproject/Makefile-local-default.mk
endif
endif

# Environment
MKDIR=mkdir -p
RM=rm -f 
MV=mv 
CP=cp 

# Macros
CND_CONF=default
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
IMAGE_TYPE=debug
OUTPUT_SUFFIX=elf
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/SoftBase.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
else
IMAGE_TYPE=production
OUTPUT_SUFFIX=hex
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/SoftBase.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
endif

# Object Directory
OBJECTDIR=build/${CND_CONF}/${IMAGE_TYPE}

# Distribution Directory
DISTDIR=dist/${CND_CONF}/${IMAGE_TYPE}

# Source Files Quoted if spaced
SOURCEFILES_QUOTED_IF_SPACED=../SoftMain.c ../../LS_API/RAW_Source/ActAcc.c ../../LS_API/RAW_Source/EEtEprom.c ../../LS_API/RAW_Source/IoTIO.c ../../LS_API/RAW_Source/LCTLcd.c ../../LS_API/RAW_Source/MtTMotors.c ../../LS_API/RAW_Source/NvTNovol.c ../../LS_API/RAW_Source/PtTProto.c ../../LS_API/RAW_Source/RfTRadio.c ../../LS_API/RAW_Source/SiTSio.c ../../LS_API/RAW_Source/TiTTImer.c ../../LS_API/RAW_Source/I2Ti2c.c ../../LS_API/LSApi.c ../../LS_API/LS_Source/LSACC.c ../../LS_API/LS_Source/LSCOM.c ../../LS_API/LS_Source/LSExec.c ../../LS_API/LS_Source/LSIO.c ../../LS_API/LS_Source/LSLcd.c ../../LS_API/LS_Source/LSMotors.c ../../LS_API/LS_Source/LSNVOL.c ../../LS_API/LS_Source/LSUSB.c ../../LS_API/RAW_Source/memory.s

# Object Files Quoted if spaced
OBJECTFILES_QUOTED_IF_SPACED=${OBJECTDIR}/_ext/1472/SoftMain.o ${OBJECTDIR}/_ext/1168165985/ActAcc.o ${OBJECTDIR}/_ext/1168165985/EEtEprom.o ${OBJECTDIR}/_ext/1168165985/IoTIO.o ${OBJECTDIR}/_ext/1168165985/LCTLcd.o ${OBJECTDIR}/_ext/1168165985/MtTMotors.o ${OBJECTDIR}/_ext/1168165985/NvTNovol.o ${OBJECTDIR}/_ext/1168165985/PtTProto.o ${OBJECTDIR}/_ext/1168165985/RfTRadio.o ${OBJECTDIR}/_ext/1168165985/SiTSio.o ${OBJECTDIR}/_ext/1168165985/TiTTImer.o ${OBJECTDIR}/_ext/1168165985/I2Ti2c.o ${OBJECTDIR}/_ext/2014013214/LSApi.o ${OBJECTDIR}/_ext/607124742/LSACC.o ${OBJECTDIR}/_ext/607124742/LSCOM.o ${OBJECTDIR}/_ext/607124742/LSExec.o ${OBJECTDIR}/_ext/607124742/LSIO.o ${OBJECTDIR}/_ext/607124742/LSLcd.o ${OBJECTDIR}/_ext/607124742/LSMotors.o ${OBJECTDIR}/_ext/607124742/LSNVOL.o ${OBJECTDIR}/_ext/607124742/LSUSB.o ${OBJECTDIR}/_ext/1168165985/memory.o
POSSIBLE_DEPFILES=${OBJECTDIR}/_ext/1472/SoftMain.o.d ${OBJECTDIR}/_ext/1168165985/ActAcc.o.d ${OBJECTDIR}/_ext/1168165985/EEtEprom.o.d ${OBJECTDIR}/_ext/1168165985/IoTIO.o.d ${OBJECTDIR}/_ext/1168165985/LCTLcd.o.d ${OBJECTDIR}/_ext/1168165985/MtTMotors.o.d ${OBJECTDIR}/_ext/1168165985/NvTNovol.o.d ${OBJECTDIR}/_ext/1168165985/PtTProto.o.d ${OBJECTDIR}/_ext/1168165985/RfTRadio.o.d ${OBJECTDIR}/_ext/1168165985/SiTSio.o.d ${OBJECTDIR}/_ext/1168165985/TiTTImer.o.d ${OBJECTDIR}/_ext/1168165985/I2Ti2c.o.d ${OBJECTDIR}/_ext/2014013214/LSApi.o.d ${OBJECTDIR}/_ext/607124742/LSACC.o.d ${OBJECTDIR}/_ext/607124742/LSCOM.o.d ${OBJECTDIR}/_ext/607124742/LSExec.o.d ${OBJECTDIR}/_ext/607124742/LSIO.o.d ${OBJECTDIR}/_ext/607124742/LSLcd.o.d ${OBJECTDIR}/_ext/607124742/LSMotors.o.d ${OBJECTDIR}/_ext/607124742/LSNVOL.o.d ${OBJECTDIR}/_ext/607124742/LSUSB.o.d ${OBJECTDIR}/_ext/1168165985/memory.o.d

# Object Files
OBJECTFILES=${OBJECTDIR}/_ext/1472/SoftMain.o ${OBJECTDIR}/_ext/1168165985/ActAcc.o ${OBJECTDIR}/_ext/1168165985/EEtEprom.o ${OBJECTDIR}/_ext/1168165985/IoTIO.o ${OBJECTDIR}/_ext/1168165985/LCTLcd.o ${OBJECTDIR}/_ext/1168165985/MtTMotors.o ${OBJECTDIR}/_ext/1168165985/NvTNovol.o ${OBJECTDIR}/_ext/1168165985/PtTProto.o ${OBJECTDIR}/_ext/1168165985/RfTRadio.o ${OBJECTDIR}/_ext/1168165985/SiTSio.o ${OBJECTDIR}/_ext/1168165985/TiTTImer.o ${OBJECTDIR}/_ext/1168165985/I2Ti2c.o ${OBJECTDIR}/_ext/2014013214/LSApi.o ${OBJECTDIR}/_ext/607124742/LSACC.o ${OBJECTDIR}/_ext/607124742/LSCOM.o ${OBJECTDIR}/_ext/607124742/LSExec.o ${OBJECTDIR}/_ext/607124742/LSIO.o ${OBJECTDIR}/_ext/607124742/LSLcd.o ${OBJECTDIR}/_ext/607124742/LSMotors.o ${OBJECTDIR}/_ext/607124742/LSNVOL.o ${OBJECTDIR}/_ext/607124742/LSUSB.o ${OBJECTDIR}/_ext/1168165985/memory.o

# Source Files
SOURCEFILES=../SoftMain.c ../../LS_API/RAW_Source/ActAcc.c ../../LS_API/RAW_Source/EEtEprom.c ../../LS_API/RAW_Source/IoTIO.c ../../LS_API/RAW_Source/LCTLcd.c ../../LS_API/RAW_Source/MtTMotors.c ../../LS_API/RAW_Source/NvTNovol.c ../../LS_API/RAW_Source/PtTProto.c ../../LS_API/RAW_Source/RfTRadio.c ../../LS_API/RAW_Source/SiTSio.c ../../LS_API/RAW_Source/TiTTImer.c ../../LS_API/RAW_Source/I2Ti2c.c ../../LS_API/LSApi.c ../../LS_API/LS_Source/LSACC.c ../../LS_API/LS_Source/LSCOM.c ../../LS_API/LS_Source/LSExec.c ../../LS_API/LS_Source/LSIO.c ../../LS_API/LS_Source/LSLcd.c ../../LS_API/LS_Source/LSMotors.c ../../LS_API/LS_Source/LSNVOL.c ../../LS_API/LS_Source/LSUSB.c ../../LS_API/RAW_Source/memory.s


CFLAGS=
ASFLAGS=
LDLIBSOPTIONS=

############# Tool locations ##########################################
# If you copy a project from one host to another, the path where the  #
# compiler is installed may be different.                             #
# If you open this project with MPLAB X in the new host, this         #
# makefile will be regenerated and the paths will be corrected.       #
#######################################################################
# fixDeps replaces a bunch of sed/cat/printf statements that slow down the build
FIXDEPS=fixDeps

.build-conf:  ${BUILD_SUBPROJECTS}
ifneq ($(INFORMATION_MESSAGE), )
	@echo $(INFORMATION_MESSAGE)
endif
	${MAKE}  -f nbproject/Makefile-default.mk dist/${CND_CONF}/${IMAGE_TYPE}/SoftBase.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}

MP_PROCESSOR_OPTION=24FJ64GA006
MP_LINKER_FILE_OPTION=,--script="../BLp24FJ64GA006.gld"
# ------------------------------------------------------------------------------------
# Rules for buildStep: compile
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${OBJECTDIR}/_ext/1472/SoftMain.o: ../SoftMain.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/SoftMain.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/SoftMain.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../SoftMain.c  -o ${OBJECTDIR}/_ext/1472/SoftMain.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1472/SoftMain.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1    -omf=elf -O0 -I".." -I"." -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/SoftMain.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1168165985/ActAcc.o: ../../LS_API/RAW_Source/ActAcc.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1168165985" 
	@${RM} ${OBJECTDIR}/_ext/1168165985/ActAcc.o.d 
	@${RM} ${OBJECTDIR}/_ext/1168165985/ActAcc.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../../LS_API/RAW_Source/ActAcc.c  -o ${OBJECTDIR}/_ext/1168165985/ActAcc.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1168165985/ActAcc.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1    -omf=elf -O0 -I".." -I"." -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1168165985/ActAcc.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1168165985/EEtEprom.o: ../../LS_API/RAW_Source/EEtEprom.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1168165985" 
	@${RM} ${OBJECTDIR}/_ext/1168165985/EEtEprom.o.d 
	@${RM} ${OBJECTDIR}/_ext/1168165985/EEtEprom.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../../LS_API/RAW_Source/EEtEprom.c  -o ${OBJECTDIR}/_ext/1168165985/EEtEprom.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1168165985/EEtEprom.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1    -omf=elf -O0 -I".." -I"." -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1168165985/EEtEprom.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1168165985/IoTIO.o: ../../LS_API/RAW_Source/IoTIO.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1168165985" 
	@${RM} ${OBJECTDIR}/_ext/1168165985/IoTIO.o.d 
	@${RM} ${OBJECTDIR}/_ext/1168165985/IoTIO.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../../LS_API/RAW_Source/IoTIO.c  -o ${OBJECTDIR}/_ext/1168165985/IoTIO.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1168165985/IoTIO.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1    -omf=elf -O0 -I".." -I"." -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1168165985/IoTIO.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1168165985/LCTLcd.o: ../../LS_API/RAW_Source/LCTLcd.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1168165985" 
	@${RM} ${OBJECTDIR}/_ext/1168165985/LCTLcd.o.d 
	@${RM} ${OBJECTDIR}/_ext/1168165985/LCTLcd.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../../LS_API/RAW_Source/LCTLcd.c  -o ${OBJECTDIR}/_ext/1168165985/LCTLcd.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1168165985/LCTLcd.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1    -omf=elf -O0 -I".." -I"." -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1168165985/LCTLcd.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1168165985/MtTMotors.o: ../../LS_API/RAW_Source/MtTMotors.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1168165985" 
	@${RM} ${OBJECTDIR}/_ext/1168165985/MtTMotors.o.d 
	@${RM} ${OBJECTDIR}/_ext/1168165985/MtTMotors.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../../LS_API/RAW_Source/MtTMotors.c  -o ${OBJECTDIR}/_ext/1168165985/MtTMotors.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1168165985/MtTMotors.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1    -omf=elf -O0 -I".." -I"." -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1168165985/MtTMotors.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1168165985/NvTNovol.o: ../../LS_API/RAW_Source/NvTNovol.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1168165985" 
	@${RM} ${OBJECTDIR}/_ext/1168165985/NvTNovol.o.d 
	@${RM} ${OBJECTDIR}/_ext/1168165985/NvTNovol.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../../LS_API/RAW_Source/NvTNovol.c  -o ${OBJECTDIR}/_ext/1168165985/NvTNovol.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1168165985/NvTNovol.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1    -omf=elf -O0 -I".." -I"." -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1168165985/NvTNovol.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1168165985/PtTProto.o: ../../LS_API/RAW_Source/PtTProto.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1168165985" 
	@${RM} ${OBJECTDIR}/_ext/1168165985/PtTProto.o.d 
	@${RM} ${OBJECTDIR}/_ext/1168165985/PtTProto.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../../LS_API/RAW_Source/PtTProto.c  -o ${OBJECTDIR}/_ext/1168165985/PtTProto.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1168165985/PtTProto.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1    -omf=elf -O0 -I".." -I"." -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1168165985/PtTProto.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1168165985/RfTRadio.o: ../../LS_API/RAW_Source/RfTRadio.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1168165985" 
	@${RM} ${OBJECTDIR}/_ext/1168165985/RfTRadio.o.d 
	@${RM} ${OBJECTDIR}/_ext/1168165985/RfTRadio.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../../LS_API/RAW_Source/RfTRadio.c  -o ${OBJECTDIR}/_ext/1168165985/RfTRadio.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1168165985/RfTRadio.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1    -omf=elf -O0 -I".." -I"." -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1168165985/RfTRadio.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1168165985/SiTSio.o: ../../LS_API/RAW_Source/SiTSio.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1168165985" 
	@${RM} ${OBJECTDIR}/_ext/1168165985/SiTSio.o.d 
	@${RM} ${OBJECTDIR}/_ext/1168165985/SiTSio.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../../LS_API/RAW_Source/SiTSio.c  -o ${OBJECTDIR}/_ext/1168165985/SiTSio.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1168165985/SiTSio.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1    -omf=elf -O0 -I".." -I"." -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1168165985/SiTSio.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1168165985/TiTTImer.o: ../../LS_API/RAW_Source/TiTTImer.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1168165985" 
	@${RM} ${OBJECTDIR}/_ext/1168165985/TiTTImer.o.d 
	@${RM} ${OBJECTDIR}/_ext/1168165985/TiTTImer.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../../LS_API/RAW_Source/TiTTImer.c  -o ${OBJECTDIR}/_ext/1168165985/TiTTImer.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1168165985/TiTTImer.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1    -omf=elf -O0 -I".." -I"." -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1168165985/TiTTImer.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1168165985/I2Ti2c.o: ../../LS_API/RAW_Source/I2Ti2c.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1168165985" 
	@${RM} ${OBJECTDIR}/_ext/1168165985/I2Ti2c.o.d 
	@${RM} ${OBJECTDIR}/_ext/1168165985/I2Ti2c.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../../LS_API/RAW_Source/I2Ti2c.c  -o ${OBJECTDIR}/_ext/1168165985/I2Ti2c.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1168165985/I2Ti2c.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1    -omf=elf -O0 -I".." -I"." -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1168165985/I2Ti2c.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/2014013214/LSApi.o: ../../LS_API/LSApi.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/2014013214" 
	@${RM} ${OBJECTDIR}/_ext/2014013214/LSApi.o.d 
	@${RM} ${OBJECTDIR}/_ext/2014013214/LSApi.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../../LS_API/LSApi.c  -o ${OBJECTDIR}/_ext/2014013214/LSApi.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/2014013214/LSApi.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1    -omf=elf -O0 -I".." -I"." -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/2014013214/LSApi.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/607124742/LSACC.o: ../../LS_API/LS_Source/LSACC.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/607124742" 
	@${RM} ${OBJECTDIR}/_ext/607124742/LSACC.o.d 
	@${RM} ${OBJECTDIR}/_ext/607124742/LSACC.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../../LS_API/LS_Source/LSACC.c  -o ${OBJECTDIR}/_ext/607124742/LSACC.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/607124742/LSACC.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1    -omf=elf -O0 -I".." -I"." -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/607124742/LSACC.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/607124742/LSCOM.o: ../../LS_API/LS_Source/LSCOM.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/607124742" 
	@${RM} ${OBJECTDIR}/_ext/607124742/LSCOM.o.d 
	@${RM} ${OBJECTDIR}/_ext/607124742/LSCOM.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../../LS_API/LS_Source/LSCOM.c  -o ${OBJECTDIR}/_ext/607124742/LSCOM.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/607124742/LSCOM.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1    -omf=elf -O0 -I".." -I"." -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/607124742/LSCOM.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/607124742/LSExec.o: ../../LS_API/LS_Source/LSExec.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/607124742" 
	@${RM} ${OBJECTDIR}/_ext/607124742/LSExec.o.d 
	@${RM} ${OBJECTDIR}/_ext/607124742/LSExec.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../../LS_API/LS_Source/LSExec.c  -o ${OBJECTDIR}/_ext/607124742/LSExec.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/607124742/LSExec.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1    -omf=elf -O0 -I".." -I"." -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/607124742/LSExec.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/607124742/LSIO.o: ../../LS_API/LS_Source/LSIO.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/607124742" 
	@${RM} ${OBJECTDIR}/_ext/607124742/LSIO.o.d 
	@${RM} ${OBJECTDIR}/_ext/607124742/LSIO.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../../LS_API/LS_Source/LSIO.c  -o ${OBJECTDIR}/_ext/607124742/LSIO.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/607124742/LSIO.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1    -omf=elf -O0 -I".." -I"." -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/607124742/LSIO.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/607124742/LSLcd.o: ../../LS_API/LS_Source/LSLcd.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/607124742" 
	@${RM} ${OBJECTDIR}/_ext/607124742/LSLcd.o.d 
	@${RM} ${OBJECTDIR}/_ext/607124742/LSLcd.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../../LS_API/LS_Source/LSLcd.c  -o ${OBJECTDIR}/_ext/607124742/LSLcd.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/607124742/LSLcd.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1    -omf=elf -O0 -I".." -I"." -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/607124742/LSLcd.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/607124742/LSMotors.o: ../../LS_API/LS_Source/LSMotors.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/607124742" 
	@${RM} ${OBJECTDIR}/_ext/607124742/LSMotors.o.d 
	@${RM} ${OBJECTDIR}/_ext/607124742/LSMotors.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../../LS_API/LS_Source/LSMotors.c  -o ${OBJECTDIR}/_ext/607124742/LSMotors.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/607124742/LSMotors.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1    -omf=elf -O0 -I".." -I"." -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/607124742/LSMotors.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/607124742/LSNVOL.o: ../../LS_API/LS_Source/LSNVOL.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/607124742" 
	@${RM} ${OBJECTDIR}/_ext/607124742/LSNVOL.o.d 
	@${RM} ${OBJECTDIR}/_ext/607124742/LSNVOL.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../../LS_API/LS_Source/LSNVOL.c  -o ${OBJECTDIR}/_ext/607124742/LSNVOL.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/607124742/LSNVOL.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1    -omf=elf -O0 -I".." -I"." -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/607124742/LSNVOL.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/607124742/LSUSB.o: ../../LS_API/LS_Source/LSUSB.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/607124742" 
	@${RM} ${OBJECTDIR}/_ext/607124742/LSUSB.o.d 
	@${RM} ${OBJECTDIR}/_ext/607124742/LSUSB.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../../LS_API/LS_Source/LSUSB.c  -o ${OBJECTDIR}/_ext/607124742/LSUSB.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/607124742/LSUSB.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1    -omf=elf -O0 -I".." -I"." -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/607124742/LSUSB.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
else
${OBJECTDIR}/_ext/1472/SoftMain.o: ../SoftMain.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1472" 
	@${RM} ${OBJECTDIR}/_ext/1472/SoftMain.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/SoftMain.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../SoftMain.c  -o ${OBJECTDIR}/_ext/1472/SoftMain.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1472/SoftMain.o.d"        -g -omf=elf -O0 -I".." -I"." -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/SoftMain.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1168165985/ActAcc.o: ../../LS_API/RAW_Source/ActAcc.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1168165985" 
	@${RM} ${OBJECTDIR}/_ext/1168165985/ActAcc.o.d 
	@${RM} ${OBJECTDIR}/_ext/1168165985/ActAcc.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../../LS_API/RAW_Source/ActAcc.c  -o ${OBJECTDIR}/_ext/1168165985/ActAcc.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1168165985/ActAcc.o.d"        -g -omf=elf -O0 -I".." -I"." -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1168165985/ActAcc.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1168165985/EEtEprom.o: ../../LS_API/RAW_Source/EEtEprom.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1168165985" 
	@${RM} ${OBJECTDIR}/_ext/1168165985/EEtEprom.o.d 
	@${RM} ${OBJECTDIR}/_ext/1168165985/EEtEprom.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../../LS_API/RAW_Source/EEtEprom.c  -o ${OBJECTDIR}/_ext/1168165985/EEtEprom.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1168165985/EEtEprom.o.d"        -g -omf=elf -O0 -I".." -I"." -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1168165985/EEtEprom.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1168165985/IoTIO.o: ../../LS_API/RAW_Source/IoTIO.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1168165985" 
	@${RM} ${OBJECTDIR}/_ext/1168165985/IoTIO.o.d 
	@${RM} ${OBJECTDIR}/_ext/1168165985/IoTIO.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../../LS_API/RAW_Source/IoTIO.c  -o ${OBJECTDIR}/_ext/1168165985/IoTIO.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1168165985/IoTIO.o.d"        -g -omf=elf -O0 -I".." -I"." -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1168165985/IoTIO.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1168165985/LCTLcd.o: ../../LS_API/RAW_Source/LCTLcd.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1168165985" 
	@${RM} ${OBJECTDIR}/_ext/1168165985/LCTLcd.o.d 
	@${RM} ${OBJECTDIR}/_ext/1168165985/LCTLcd.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../../LS_API/RAW_Source/LCTLcd.c  -o ${OBJECTDIR}/_ext/1168165985/LCTLcd.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1168165985/LCTLcd.o.d"        -g -omf=elf -O0 -I".." -I"." -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1168165985/LCTLcd.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1168165985/MtTMotors.o: ../../LS_API/RAW_Source/MtTMotors.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1168165985" 
	@${RM} ${OBJECTDIR}/_ext/1168165985/MtTMotors.o.d 
	@${RM} ${OBJECTDIR}/_ext/1168165985/MtTMotors.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../../LS_API/RAW_Source/MtTMotors.c  -o ${OBJECTDIR}/_ext/1168165985/MtTMotors.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1168165985/MtTMotors.o.d"        -g -omf=elf -O0 -I".." -I"." -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1168165985/MtTMotors.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1168165985/NvTNovol.o: ../../LS_API/RAW_Source/NvTNovol.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1168165985" 
	@${RM} ${OBJECTDIR}/_ext/1168165985/NvTNovol.o.d 
	@${RM} ${OBJECTDIR}/_ext/1168165985/NvTNovol.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../../LS_API/RAW_Source/NvTNovol.c  -o ${OBJECTDIR}/_ext/1168165985/NvTNovol.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1168165985/NvTNovol.o.d"        -g -omf=elf -O0 -I".." -I"." -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1168165985/NvTNovol.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1168165985/PtTProto.o: ../../LS_API/RAW_Source/PtTProto.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1168165985" 
	@${RM} ${OBJECTDIR}/_ext/1168165985/PtTProto.o.d 
	@${RM} ${OBJECTDIR}/_ext/1168165985/PtTProto.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../../LS_API/RAW_Source/PtTProto.c  -o ${OBJECTDIR}/_ext/1168165985/PtTProto.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1168165985/PtTProto.o.d"        -g -omf=elf -O0 -I".." -I"." -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1168165985/PtTProto.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1168165985/RfTRadio.o: ../../LS_API/RAW_Source/RfTRadio.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1168165985" 
	@${RM} ${OBJECTDIR}/_ext/1168165985/RfTRadio.o.d 
	@${RM} ${OBJECTDIR}/_ext/1168165985/RfTRadio.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../../LS_API/RAW_Source/RfTRadio.c  -o ${OBJECTDIR}/_ext/1168165985/RfTRadio.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1168165985/RfTRadio.o.d"        -g -omf=elf -O0 -I".." -I"." -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1168165985/RfTRadio.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1168165985/SiTSio.o: ../../LS_API/RAW_Source/SiTSio.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1168165985" 
	@${RM} ${OBJECTDIR}/_ext/1168165985/SiTSio.o.d 
	@${RM} ${OBJECTDIR}/_ext/1168165985/SiTSio.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../../LS_API/RAW_Source/SiTSio.c  -o ${OBJECTDIR}/_ext/1168165985/SiTSio.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1168165985/SiTSio.o.d"        -g -omf=elf -O0 -I".." -I"." -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1168165985/SiTSio.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1168165985/TiTTImer.o: ../../LS_API/RAW_Source/TiTTImer.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1168165985" 
	@${RM} ${OBJECTDIR}/_ext/1168165985/TiTTImer.o.d 
	@${RM} ${OBJECTDIR}/_ext/1168165985/TiTTImer.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../../LS_API/RAW_Source/TiTTImer.c  -o ${OBJECTDIR}/_ext/1168165985/TiTTImer.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1168165985/TiTTImer.o.d"        -g -omf=elf -O0 -I".." -I"." -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1168165985/TiTTImer.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1168165985/I2Ti2c.o: ../../LS_API/RAW_Source/I2Ti2c.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1168165985" 
	@${RM} ${OBJECTDIR}/_ext/1168165985/I2Ti2c.o.d 
	@${RM} ${OBJECTDIR}/_ext/1168165985/I2Ti2c.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../../LS_API/RAW_Source/I2Ti2c.c  -o ${OBJECTDIR}/_ext/1168165985/I2Ti2c.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/1168165985/I2Ti2c.o.d"        -g -omf=elf -O0 -I".." -I"." -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/1168165985/I2Ti2c.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/2014013214/LSApi.o: ../../LS_API/LSApi.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/2014013214" 
	@${RM} ${OBJECTDIR}/_ext/2014013214/LSApi.o.d 
	@${RM} ${OBJECTDIR}/_ext/2014013214/LSApi.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../../LS_API/LSApi.c  -o ${OBJECTDIR}/_ext/2014013214/LSApi.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/2014013214/LSApi.o.d"        -g -omf=elf -O0 -I".." -I"." -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/2014013214/LSApi.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/607124742/LSACC.o: ../../LS_API/LS_Source/LSACC.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/607124742" 
	@${RM} ${OBJECTDIR}/_ext/607124742/LSACC.o.d 
	@${RM} ${OBJECTDIR}/_ext/607124742/LSACC.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../../LS_API/LS_Source/LSACC.c  -o ${OBJECTDIR}/_ext/607124742/LSACC.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/607124742/LSACC.o.d"        -g -omf=elf -O0 -I".." -I"." -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/607124742/LSACC.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/607124742/LSCOM.o: ../../LS_API/LS_Source/LSCOM.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/607124742" 
	@${RM} ${OBJECTDIR}/_ext/607124742/LSCOM.o.d 
	@${RM} ${OBJECTDIR}/_ext/607124742/LSCOM.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../../LS_API/LS_Source/LSCOM.c  -o ${OBJECTDIR}/_ext/607124742/LSCOM.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/607124742/LSCOM.o.d"        -g -omf=elf -O0 -I".." -I"." -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/607124742/LSCOM.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/607124742/LSExec.o: ../../LS_API/LS_Source/LSExec.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/607124742" 
	@${RM} ${OBJECTDIR}/_ext/607124742/LSExec.o.d 
	@${RM} ${OBJECTDIR}/_ext/607124742/LSExec.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../../LS_API/LS_Source/LSExec.c  -o ${OBJECTDIR}/_ext/607124742/LSExec.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/607124742/LSExec.o.d"        -g -omf=elf -O0 -I".." -I"." -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/607124742/LSExec.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/607124742/LSIO.o: ../../LS_API/LS_Source/LSIO.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/607124742" 
	@${RM} ${OBJECTDIR}/_ext/607124742/LSIO.o.d 
	@${RM} ${OBJECTDIR}/_ext/607124742/LSIO.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../../LS_API/LS_Source/LSIO.c  -o ${OBJECTDIR}/_ext/607124742/LSIO.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/607124742/LSIO.o.d"        -g -omf=elf -O0 -I".." -I"." -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/607124742/LSIO.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/607124742/LSLcd.o: ../../LS_API/LS_Source/LSLcd.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/607124742" 
	@${RM} ${OBJECTDIR}/_ext/607124742/LSLcd.o.d 
	@${RM} ${OBJECTDIR}/_ext/607124742/LSLcd.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../../LS_API/LS_Source/LSLcd.c  -o ${OBJECTDIR}/_ext/607124742/LSLcd.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/607124742/LSLcd.o.d"        -g -omf=elf -O0 -I".." -I"." -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/607124742/LSLcd.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/607124742/LSMotors.o: ../../LS_API/LS_Source/LSMotors.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/607124742" 
	@${RM} ${OBJECTDIR}/_ext/607124742/LSMotors.o.d 
	@${RM} ${OBJECTDIR}/_ext/607124742/LSMotors.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../../LS_API/LS_Source/LSMotors.c  -o ${OBJECTDIR}/_ext/607124742/LSMotors.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/607124742/LSMotors.o.d"        -g -omf=elf -O0 -I".." -I"." -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/607124742/LSMotors.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/607124742/LSNVOL.o: ../../LS_API/LS_Source/LSNVOL.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/607124742" 
	@${RM} ${OBJECTDIR}/_ext/607124742/LSNVOL.o.d 
	@${RM} ${OBJECTDIR}/_ext/607124742/LSNVOL.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../../LS_API/LS_Source/LSNVOL.c  -o ${OBJECTDIR}/_ext/607124742/LSNVOL.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/607124742/LSNVOL.o.d"        -g -omf=elf -O0 -I".." -I"." -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/607124742/LSNVOL.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/607124742/LSUSB.o: ../../LS_API/LS_Source/LSUSB.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/607124742" 
	@${RM} ${OBJECTDIR}/_ext/607124742/LSUSB.o.d 
	@${RM} ${OBJECTDIR}/_ext/607124742/LSUSB.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  ../../LS_API/LS_Source/LSUSB.c  -o ${OBJECTDIR}/_ext/607124742/LSUSB.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/607124742/LSUSB.o.d"        -g -omf=elf -O0 -I".." -I"." -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/_ext/607124742/LSUSB.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: assemble
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${OBJECTDIR}/_ext/1168165985/memory.o: ../../LS_API/RAW_Source/memory.s  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1168165985" 
	@${RM} ${OBJECTDIR}/_ext/1168165985/memory.o.d 
	@${RM} ${OBJECTDIR}/_ext/1168165985/memory.o 
	${MP_CC} $(MP_EXTRA_AS_PRE)  ../../LS_API/RAW_Source/memory.s  -o ${OBJECTDIR}/_ext/1168165985/memory.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -omf=elf -I".." -I"." -Wa,-MD,"${OBJECTDIR}/_ext/1168165985/memory.o.d",--defsym=__MPLAB_BUILD=1,--defsym=__ICD2RAM=1,--defsym=__MPLAB_DEBUG=1,--defsym=__DEBUG=1,--defsym=__MPLAB_DEBUGGER_PK3=1,-g,--no-relax,-g$(MP_EXTRA_AS_POST)
	@${FIXDEPS} "${OBJECTDIR}/_ext/1168165985/memory.o.d"  $(SILENT)  -rsi ${MP_CC_DIR}../  
	
else
${OBJECTDIR}/_ext/1168165985/memory.o: ../../LS_API/RAW_Source/memory.s  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1168165985" 
	@${RM} ${OBJECTDIR}/_ext/1168165985/memory.o.d 
	@${RM} ${OBJECTDIR}/_ext/1168165985/memory.o 
	${MP_CC} $(MP_EXTRA_AS_PRE)  ../../LS_API/RAW_Source/memory.s  -o ${OBJECTDIR}/_ext/1168165985/memory.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -omf=elf -I".." -I"." -Wa,-MD,"${OBJECTDIR}/_ext/1168165985/memory.o.d",--defsym=__MPLAB_BUILD=1,-g,--no-relax,-g$(MP_EXTRA_AS_POST)
	@${FIXDEPS} "${OBJECTDIR}/_ext/1168165985/memory.o.d"  $(SILENT)  -rsi ${MP_CC_DIR}../  
	
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: assemblePreproc
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: link
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
dist/${CND_CONF}/${IMAGE_TYPE}/SoftBase.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk    ../BLp24FJ64GA006.gld
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_CC} $(MP_EXTRA_LD_PRE)  -o dist/${CND_CONF}/${IMAGE_TYPE}/SoftBase.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}  ${OBJECTFILES_QUOTED_IF_SPACED}      -mcpu=$(MP_PROCESSOR_OPTION)        -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1  -omf=elf  -mreserve=data@0x800:0x81F -mreserve=data@0x820:0x821 -mreserve=data@0x822:0x823 -mreserve=data@0x824:0x825 -mreserve=data@0x826:0x84F   -Wl,,--defsym=__MPLAB_BUILD=1,--defsym=__MPLAB_DEBUG=1,--defsym=__DEBUG=1,--defsym=__MPLAB_DEBUGGER_PK3=1,$(MP_LINKER_FILE_OPTION),--stack=16,--check-sections,--data-init,--pack-data,--handles,--isr,--no-gc-sections,--fill-upper=0,--stackguard=16,--library-path="..",--library-path=".",--no-force-link,--smart-io,-Map="${DISTDIR}/SoftBase.X.${IMAGE_TYPE}.map",--report-mem$(MP_EXTRA_LD_POST) 
	
else
dist/${CND_CONF}/${IMAGE_TYPE}/SoftBase.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk   ../BLp24FJ64GA006.gld
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_CC} $(MP_EXTRA_LD_PRE)  -o dist/${CND_CONF}/${IMAGE_TYPE}/SoftBase.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX}  ${OBJECTFILES_QUOTED_IF_SPACED}      -mcpu=$(MP_PROCESSOR_OPTION)        -omf=elf -Wl,,--defsym=__MPLAB_BUILD=1,$(MP_LINKER_FILE_OPTION),--stack=16,--check-sections,--data-init,--pack-data,--handles,--isr,--no-gc-sections,--fill-upper=0,--stackguard=16,--library-path="..",--library-path=".",--no-force-link,--smart-io,-Map="${DISTDIR}/SoftBase.X.${IMAGE_TYPE}.map",--report-mem$(MP_EXTRA_LD_POST) 
	${MP_CC_DIR}/xc16-bin2hex dist/${CND_CONF}/${IMAGE_TYPE}/SoftBase.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX} -a  -omf=elf  
	
endif


# Subprojects
.build-subprojects:


# Subprojects
.clean-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r build/default
	${RM} -r dist/default

# Enable dependency checking
.dep.inc: .depcheck-impl

DEPFILES=$(shell "${PATH_TO_IDE_BIN}"mplabwildcard ${POSSIBLE_DEPFILES})
ifneq (${DEPFILES},)
include ${DEPFILES}
endif
