#pragma once

enum class eMessageType
{
	NONE,

	/*************
		UI 관련
	**************/
	RECIEVE_ACTOR,
	RECIEVE_TOOL,
	SAVE_ACTOR,
	LOAD_ACTOR,
	CLICK_START,

	LOAD_ENEMY,

	SET_FREE_POINT,
	SET_EDITED_ACTOR_THIRD_POINT,
	SET_ENEMY_ACTOR_THIRD_POINT,

	/***********************
		인게임 초기화 관련
	************************/
	REGISTER_ACTOR,
};