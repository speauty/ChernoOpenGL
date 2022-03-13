#shader vertex
#version 330 core

layout(location = 0)in vec4 position;
layout(location = 1)in vec4 a_Color;
layout(location = 2)in vec2 a_TextCoord;
layout(location = 3)in float a_TextSlotIdx;

out vec4 v_Color;
out vec2 v_TextCoord;
out float v_TextSlotIdx;

uniform mat4 u_MVP;

void main()
{
	gl_Position = u_MVP * position;
	v_Color = a_Color;
	v_TextCoord = a_TextCoord;
	v_TextSlotIdx = a_TextSlotIdx;
};

#shader fragment
#version 330 core

layout(location = 0)out vec4 o_Color;

in vec4 v_Color;
in vec2 v_TextCoord;
in float v_TextSlotIdx;

uniform sampler2D u_Textures[2];

void main()
{
	int idx = int(v_TextSlotIdx);
	vec4 textColor = texture(u_Textures[idx], v_TextCoord);
	o_Color = textColor;
};