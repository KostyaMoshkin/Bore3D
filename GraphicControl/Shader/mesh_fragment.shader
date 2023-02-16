#version 460 core

uniform vec3 m_vMesColor;


out vec4 color;

void main()
{
	color = vec4(m_vMesColor, 1.0);
}