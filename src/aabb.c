#include "../include/aabb.h"


aabb_buffer_t aabb_buffers[AABB_BUFFER_COUNT];
GLuint aabb_shader;

int
aabbs_init(void)
{
	GLuint vshader, fshader;
	vec3_t vertices[8] = {
		{{ 0.0, 0.0, 0.0 }}, // 0 
		{{ 1.0, 0.0, 0.0 }}, // 1
		{{ 1.0, 1.0, 0.0 }}, // 2
		{{ 0.0, 1.0, 0.0 }}, // 3
		{{ 0.0, 0.0, 1.0 }}, // 4
		{{ 1.0, 0.0, 1.0 }}, // 5
		{{ 1.0, 1.0, 1.0 }}, // 6
		{{ 0.0, 1.0, 1.0 }}, // 7
	};

	GLuint indices[36] = {
		// front face
		0, 1, 2,
		2, 3, 0,

		// left face
		0, 4, 7,
		7, 3, 0,
		//
		
		// right face
		1, 5, 6,
		6, 2, 1,
		
		// back face
		
		5, 4, 7,
		7, 6, 5,
		
		// top face
		4, 5, 1,
		1, 0, 4,
		
		// bottom face
		3, 2, 6,
		6, 7, 3,
	};

	GLchar *vsource = "#version 450 core\n"
		"uniform mat4 model;"
		"uniform mat4 view;"
		"uniform mat4 projection;"
		"layout (location = 0) in vec3 vertex;"
		"void main()"
		"{"
		"gl_Position = projection * view * model * vec4(vertex, 1.0);"
		"}";

	GLchar *fsource = "#version 450 core\n"
		"uniform vec4 colour;"
		"out vec4 final_colour;"
		"void main()"
		"{"
		"final_colour = colour;"
		"}";

	vshader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vshader, 1, (const GLchar **) &vsource, NULL);
	glCompileShader(vshader);
	
	fshader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fshader, 1, (const GLchar **) &fsource, NULL);
	glCompileShader(fshader);

	aabb_shader = glCreateProgram();
	glAttachShader(aabb_shader, vshader);
	glAttachShader(aabb_shader, fshader);
	glLinkProgram(aabb_shader);
	
	glGenVertexArrays(1, aabb_buffers+AABB_BUFFER_VAO);
	glBindVertexArray(aabb_buffers[AABB_BUFFER_VAO]);

	glGenBuffers(2, aabb_buffers+AABB_BUFFER_VBO);
	glBindBuffer(GL_ARRAY_BUFFER, aabb_buffers[AABB_BUFFER_VBO]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, aabb_buffers[AABB_BUFFER_EBO]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);
	
	glBindVertexArray(0);
	glDisableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);	
	return 0;
}

aabb_t
aabb_empty(void)
{
	return (aabb_t) {
		{{0.0, 0.0, 0.0}},
		{{0.0, 0.0, 0.0}}
	};
}

int
aabb_contains(aabb_t a, aabb_t b)
{
	for (int i = 0; i < 3; i++) {
		if (!((b.min.data[i] >= a.min.data[i] ||
		       fabsf(b.min.data[i] - a.min.data[i]) < 0.001) &&
		      (b.max.data[i] <= a.max.data[i] ||
		       fabsf(b.max.data[i] - a.max.data[i]) < 0.001))) return 0;
	}
	return 1;
}

// Slab Method for AABB - Ray Intersection. 
vec2_t
aabb_ray_intersect(ray_t ray, aabb_t aabb)
{
	int i;
	float t0, t1;
	float tmin, tmax;
	
	tmin = 0, tmax = INFINITY;
	for (i = 0; i < 3; i++) {
		t0 = (aabb.min.data[i] - ray.origin.data[i]) / ray.direction.data[i];
		t1 = (aabb.max.data[i] - ray.origin.data[i]) / ray.direction.data[i];

		tmin = fminf(fmaxf(t0, tmin), fmaxf(t1, tmin));
		tmax = fmaxf(fminf(t0, tmax), fminf(t1, tmax));
	}

	return ll_vec2_create2f(tmin, tmax);
}

int
aabb_ray_hit(ray_t ray, aabb_t aabb)
{
	vec2_t intersect = aabb_ray_intersect(ray, aabb);
	return intersect.x <= intersect.y;
}
