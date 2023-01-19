#version 460 core

uniform sampler1D m_tPalette;

smooth in float fPaletteIndex;

void main()
{
	gl_FragColor = vec4(texture(m_tPalette, max(min(fPaletteIndex, 0.92), 0.08)).rgb, 1.0);
}