/* IRLib_P99_Additional.h
 * Part of IRLib Library for Arduino receiving, decoding, and sending
 * infrared signals. See COPYRIGHT.txt and LICENSE.txt for more information.
 */
/*
 * This is dummy code that you can copy and rename and modify when implementing new protocols.
 */

#ifndef IRLIB_PROTOCOL_99_H
#define IRLIB_PROTOCOL_99_H
#define IR_SEND_PROTOCOL_99		case 99: IRsendAdditional::send(data); break;
#define IR_DECODE_PROTOCOL_99	if(IRdecodeAdditional::decode()) return true;
#ifdef IRLIB_HAVE_COMBO
	#define PV_IR_DECODE_PROTOCOL_99 ,public virtual IRdecodeAdditional
	#define PV_IR_SEND_PROTOCOL_99   ,public virtual IRsendAdditional
#else
	#define PV_IR_DECODE_PROTOCOL_99  public virtual IRdecodeAdditional
	#define PV_IR_SEND_PROTOCOL_99    public virtual IRsendAdditional
#endif

#ifdef IRLIBSENDBASE_H
class IRsendAdditional: public virtual IRsendBase {
  public:
    void IRsendAdditional::send(uint32_t data, uint8_t nbits) {
      for(uint8_t i=0; i<3;i++){
        sendGeneric(data,nbits, 600*4, 600, 600*2, 600, 600, 600, 40, false,45000); 
      }
    };
};
#endif  //IRLIBSENDBASE_H

#ifdef IRLIBDECODEBASE_H
class IRdecodeAdditional: public virtual IRdecodeBase {
  public:
    bool IRdecodeAdditional::decode(void) {
            IRLIB_ATTEMPT_MESSAGE(F("Additional"));
      resetDecoder();//This used to be in the receiver getResults.
      if(recvGlobal.decodeLength!=2*8+2 && recvGlobal.decodeLength!=2*12+2 && recvGlobal.decodeLength!=2*15+2 
        && recvGlobal.decodeLength!=2*20+2) return RAW_COUNT_ERROR;
      if(!ignoreHeader) {
        if (!MATCH(recvGlobal.decodeBuffer[1],600*4)) return HEADER_MARK_ERROR(600*4);
      }
      offset=2;//skip initial gap plus header Mark.
      while (offset < recvGlobal.decodeLength) {
        if (!MATCH(recvGlobal.decodeBuffer[offset],600)) return DATA_SPACE_ERROR(600);
        offset++;
        if (MATCH(recvGlobal.decodeBuffer[offset],600*2)) {
          value = (value << 1) | 1;
        } 
        else if (MATCH(recvGlobal.decodeBuffer[offset],600)) {
          value <<= 1;
        } 
        else return DATA_MARK_ERROR(600);
        offset++;
      }
      bits = (offset - 1) / 2;
      protocolNum = ADDITIONAL;
      return true;
    }
};

#endif //IRLIBDECODEBASE_H

#define IRLIB_HAVE_COMBO

#endif //IRLIB_PROTOCOL_99_H