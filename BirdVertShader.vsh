// Uniform variable values are set before the draw by the CPU.
// They are constant across a given draw.
uniform mat4 pvmMatrix;

// Attributes generally come from buffers.
// Every time this main() runs, a new vertex is loaded into these buffers.
attribute vec3 position;
attribute vec3 color;
attribute vec3 normal;

// Varying variables are passed out of the vertex shader into the pipeline
varying vec3 vColor;
varying vec3 vNorm;

void main(void)
{
	// This sets the varying color to match the color attribute of the current vertex
	vColor = color;
	vNorm = vec3(pvmMatrix * vec4(normal, 0.0));

	// This applies the projection matrix to the position attribute,
	// and sets the final position (gl_Position) to the transformed value
	gl_Position = pvmMatrix * vec4(position, 1.0);
}
