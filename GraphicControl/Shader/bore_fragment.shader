#version 460 core

uniform sampler1D m_tPalette;

smooth in float fPaletteIndex;

void main()
{
	gl_FragColor = vec4(texture(m_tPalette, fPaletteIndex).rgb, 1.0);
	//gl_FragColor = vec4(1.0);
}