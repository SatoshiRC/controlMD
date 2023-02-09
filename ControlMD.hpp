/*
 * ControlMD.hpp
 *
 *  Created on: Oct 11, 2019
 *      Author: Satoshi Ohya
 */
#ifndef CONTROL_MD_HPP_
#define CONTROL_MD_HPP_

#include"tim.h"

class ControlMD{
public:
  ControlMD(){};
  ControlMD(TIM_HandleTypeDef *pTim, uint32_t pChannel, TIM_HandleTypeDef *nTim, uint32_t nChannel)
  :pTim(pTim),pChannel(pChannel),nTim(nTim),nChannel(nChannel){};
  void init(float maxSpeedPercent);
  void start();
  void stop();
  void resetSpeed();
  uint8_t setSpeed(int16_t speed);
  void setSpeedPercent(int8_t percent);

private:
  TIM_HandleTypeDef *pTim;
  uint32_t pChannel;
  TIM_HandleTypeDef *nTim;
  uint32_t nChannel;
  uint16_t maxCounterPeriod;

};

#endif /*CONTROL_MD_HPP_*/
