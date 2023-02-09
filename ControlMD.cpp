/*
 * ControlMD.cpp
 *
 *  Created on: Oct 11, 2019
 *      Author: Satoshi Ohya
 */

 #include"ControlMD.hpp"

void ControlMD::init(float maxSpeedPercent){
	if(pTim->Instance->PSC >= nTim->Instance->PSC){
		maxCounterPeriod = (uint16_t)nTim->Init.Period * maxSpeedPercent / 100.0;
	}else{
		maxCounterPeriod = (uint16_t)pTim->Init.Period * maxSpeedPercent / 100.0;
  	}
}

void ControlMD::start(){
	HAL_TIM_PWM_Start(pTim, pChannel);
	HAL_TIM_PWM_Start(nTim, nChannel);
	resetSpeed();
}

void ControlMD::stop(){
	resetSpeed();
	HAL_TIM_PWM_Stop(pTim, pChannel);
	HAL_TIM_PWM_Stop(nTim, nChannel);
}

void ControlMD::resetSpeed(){
	__HAL_TIM_SET_COMPARE(pTim, pChannel, 0);
	__HAL_TIM_SET_COMPARE(nTim, nChannel, 0);
}

uint8_t ControlMD::setSpeed(int16_t speed){
	if(speed < -maxCounterPeriod or maxCounterPeriod < speed){
    	resetSpeed();
    	return 1;
	}
	if(speed < 0){
		__HAL_TIM_SET_COMPARE(pTim, pChannel, 0);
		__HAL_TIM_SET_COMPARE(nTim, nChannel, -speed);
	} else if(0 < speed){
		__HAL_TIM_SET_COMPARE(pTim, pChannel, speed);
		__HAL_TIM_SET_COMPARE(nTim, nChannel, 0);
	} else {
		__HAL_TIM_SET_COMPARE(pTim, pChannel, 0);
		__HAL_TIM_SET_COMPARE(nTim, nChannel, 0);
	}	
	return 0;
}

void ControlMD::setSpeedPercent(int8_t percent){
	setSpeed((int16_t)percent * MD[number].maxCounterPeriod * 0.01);
}
