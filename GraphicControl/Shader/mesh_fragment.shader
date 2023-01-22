#version 460 core

uniform vec3 m_vMesColor;

void main()
{
	gl_FragColor = vec4(m_vMesColor, 1.0);
}