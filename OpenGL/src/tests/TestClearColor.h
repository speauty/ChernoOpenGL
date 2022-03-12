#pragma once

#include "Test.h"

namespace test
{

	/* 清除颜色: 指的是用指定颜色填充背景色 刷屏 */
	class TestClearColor : public Test
	{
	private:
		const char* m_Label;
		float m_ClearColor[4];
	public:
		TestClearColor();
		~TestClearColor();

		void OnUpdate(float deltaTime) override;
		void OnRender() override;
		void OnImGuiRender() override;
	};
}