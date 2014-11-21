// This is the same varying variable as the one in the vertex shader.
// However, this variable is an input to this stage, whereas it is
// an output of the vertex shading stage.
// Every vertex has a color. We want each pixel to have a color,
// so we pass vColor out of the vertex shading stage.
// Then, three vertices are formed into a triangle.
// Then that triangle is turned into pixels.
// Each pixel gets a color, but what color? It is an
// interpolation of the 3 colors defined by the triangle.
varying vec3 vColor;
varying vec3 vNorm;

void main(void)
{

	vec3 lightPosition = vec3(1, 1, 0);
	float light = dot(normalize(vNorm), normalize(lightPosition));
	light = light / 2.0 + 0.5;

	vec3 shadowColor = vec3(0.2, 0.2, 0.2);
	vec3 finalColor = mix(shadowColor, vColor, light);

	// This is the only output of the fragment shader.
	// It is the final pixel color (try changing it)
	// gl_FragColor = vec4(1.0, 0.0, 1.0, 1.0);
	// The 4th value is alpha, which deals with transparency.
	gl_FragColor = vec4(finalColor, 1.0);
}
