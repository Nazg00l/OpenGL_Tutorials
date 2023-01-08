#version 330 core
layout ( location = 0 ) in vec3 position;
//layout ( location = 1 ) in vec3 color;
layout ( location = 2 ) in vec2 texCoord;

out vec2 TexCoord;

uniform mat4 model;			// Converts local object coordinates to Camera coordinates
uniform mat4 projection;	// Converts Camera coordinates to normalized coordinates (means coordinates between 0 and 1)
uniform mat4 view;			// Converts the the normalized to Window coordinates (aka your screen, your window )


void main ()
{
	gl_Position = projection * view * model * vec4(position, 1.0f);

	TexCoord = vec2( texCoord.x, 1.0 - texCoord.y ); // Swapping the y-axis by subtracting our coordinates from 1 
													// and this is because most images have the top y-axis inverted in opengl 
}

