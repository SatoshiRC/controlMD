/*
 * ControlMD.cpp
 *
 *  Created on: Oct 11, 2019
 *      Author: Satoshi Ohya
 */

 #include"ControlMD.hpp"

 ControlMD::ControlMD(){

 }

 ControlMD::~ControlMD(){
	 MD.clear();
 }

void ControlMD::addMD(uint8_t number, TIM_HandleTypeDef *pTim, uint32_t pChannel, TIM_HandleTypeDef *nTim, uint32_t nChannel){
  MD_t tmpArgument = {pTim , pChannel , nTim , nChannel , 0};
  MD.insert(std::make_pair(number, tmpArgument));
}

void ControlMD::init(uint8_t number){
  if(MD[number].pTim->Instance->PSC >= MD[number].nTim->Instance->PSC){
	  MD[number].maxCounterPeriod = MD[number].nTim->Instance->PSC * 0.98;
  }else{
	  MD[number].maxCounterPeriod = MD[number].pTim->Instance->PSC * 0.98;
  }
}

void ControlMD::start(uint8_t number){
	HAL_TIM_PWM_Start(MD[number].pTim, MD[number].pChannel);
	HAL_TIM_PWM_Start(MD[number].nTim, MD[number].nChannel);
	resetSpeed(number);
}

void ControlMD::stop(uint8_t number){
	resetSpeed(number);
	HAL_TIM_PWM_Stop(MD[number].pTim, MD[number].pChannel);
	HAL_TIM_PWM_Stop(MD[number].nTim, MD[number].nChannel);
}

void ControlMD::resetSpeed(uint8_t number){
	__HAL_TIM_SET_COMPARE(MD[number].pTim, MD[number].pChannel, 0);
	__HAL_TIM_SET_COMPARE(MD[number].nTim, MD[number].nChannel, 0);
}

void ControlMD::setSpeed(uint8_t number, int16_t speed){
	if(speed < -MD[number].maxCounterPeriod or MD[number].maxCounterPeriod < speed) return;
	if(speed < 0){
		__HAL_TIM_SET_COMPARE(MD[number].pTim, MD[number].pChannel, 0);
		__HAL_TIM_SET_COMPARE(MD[number].nTim, MD[number].nChannel, -speed);
	} else if(0 < speed){
		__HAL_TIM_SET_COMPARE(MD[number].pTim, MD[number].pChannel, speed);
		__HAL_TIM_SET_COMPARE(MD[number].nTim, MD[number].nChannel, 0);
	} else {
		__HAL_TIM_SET_COMPARE(MD[number].pTim, MD[number].pChannel, 0);
		__HAL_TIM_SET_COMPARE(MD[number].nTim, MD[number].nChannel, 0);
	}
}

void ControlMD::setSpeedPercent(uint8_t number , int8_t percent){
	setSpeed(number, (int16_t)percent * MD[number].maxCounterPeriod * 0.01);
}