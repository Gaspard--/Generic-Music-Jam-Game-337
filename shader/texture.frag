#version 130

uniform sampler2D tex;

in vec2 vert_coord
out vec4 color;

void main()
{
	out = texture(tex, vert_coord);
}
