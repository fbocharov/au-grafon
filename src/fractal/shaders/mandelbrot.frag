#version 330 core

uniform vec2 camera;
uniform float zoom;

uniform vec2 screen;
uniform float maxIterCount;

uniform sampler1D tex;

void main()
{
	vec2 z = vec2(0.0, 0.0);
	vec2 c = vec2(camera.x + (gl_FragCoord.x / screen.x) * zoom,
	              camera.y + (1.0 - gl_FragCoord.y / screen.y) * zoom);

	float iterCount = 0.0f;
	while (iterCount < maxIterCount) {
		z = vec2(z.x * z.x - z.y * z.y + c.x,
		         2.0 * z.x * z.y + c.y);

		if (z.x * z.x + z.y * z.y > 4.0)
			break;

		iterCount += 1.0;
	}

	if (iterCount > maxIterCount - 0.0001) {
		gl_FragColor = vec4(0.0, 0.0, 0.0, 1.0);
	} else {
		gl_FragColor = texture(tex, iterCount / maxIterCount).rgba;
	}
}
