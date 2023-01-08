#version 330 core
in vec2 TexCoord;

out vec4 color;

uniform sampler2D ourTexture1;

uniform vec4 ourColor;
uniform bool isChangeColor; // this is whether to change the color over the texture or not

void main()
{
	//color = texture(ourTexture1, TexCoord);
	//color = texture(ourTexture1, TexCoord) * ourColor;
	if (isChangeColor)
		color = texture(ourTexture1, TexCoord) * ourColor;
	else
		color = texture(ourTexture1, TexCoord);
}