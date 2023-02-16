#version 460 core

uniform sampler1D m_tPalette;
uniform float m_fPalleteLimit;

smooth in float fPaletteIndex;

out vec4 color;

void main()
{
	color = vec4(texture(m_tPalette, max(min(fPaletteIndex, 1.0 - m_fPalleteLimit), m_fPalleteLimit)).rgb, 1.0);
}