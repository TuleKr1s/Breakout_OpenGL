#include "Post_Processor.h"

PostProcessor::PostProcessor(Shader shader, unsigned int width, unsigned int height)
	: PostProcessingShader(shader), m_width(width), m_height(height), confuse(false), chaos(false), shake(false) 
{
	glGenFramebuffers(1, &MSFBO);
    glGenFramebuffers(1, &FBO);
    glGenRenderbuffers(1, &RBO);

	glBindFramebuffer(GL_FRAMEBUFFER, MSFBO);
	glBindRenderbuffer(GL_RENDERBUFFER, RBO);
	glRenderbufferStorageMultisample(GL_RENDERBUFFER, 4, GL_RGB, m_width, m_height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, RBO);
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cerr << "ERROR::POSTPROCESSOR: Failed to initialize MSFBO" << std::endl;

	glBindFramebuffer(GL_FRAMEBUFFER, FBO);
	m_texture.generate(m_width, m_height, NULL);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_texture.ID, 0);
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cerr << "ERROR::POSTPROCESSOR: Failed to initialize FBO" << std::endl;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	initRenderData();
	PostProcessingShader.setInt("scene", 0, true);
	float offset = 1.0f / 300.0f;
	float offsets[9][2] {
		{ -offset,  offset  },  // up-left
		{  0.0f,    offset  },  // up-center
		{  offset,  offset  },  // up-right
		{ -offset,  0.0f    },  // center-left
		{  0.0f,    0.0f    },  // center-center
		{  offset,  0.0f    },  // center-right
		{ -offset, -offset  },  // down-left
		{  0.0f,   -offset  },  // down-center
		{  offset, -offset  }   // down-right
	};
	glUniform2fv(glGetUniformLocation(PostProcessingShader.ID, "offsets"), 9, (float*)offsets);
	int edge_kernel[9]{
		-1, -1, -1,
		-1, 8, -1,
		-1, -1, -1
	};
	glUniform1iv(glGetUniformLocation(PostProcessingShader.ID, "edge_kernel"), 9, edge_kernel);
	float blur_kernel[9]{
		1.0f / 16.0f, 2.0f / 16.0f, 1.0f / 16.0f,
		2.0f / 16.0f, 4.0f / 16.0f, 2.0f / 16.0f,
		1.0f / 16.0f, 2.0f / 16.0f, 1.0f / 16.0f
	};
	glUniform1fv(glGetUniformLocation(PostProcessingShader.ID, "blur_kernel"), 9, blur_kernel);
}

void PostProcessor::beginRender() {
	glBindFramebuffer(GL_FRAMEBUFFER, MSFBO);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
}

void PostProcessor::endRender() {
	glBindFramebuffer(GL_READ_FRAMEBUFFER, MSFBO);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, FBO);
	glBlitFramebuffer(0, 0, m_width, m_height, 0, 0, m_width, m_height, GL_COLOR_BUFFER_BIT, GL_NEAREST);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void PostProcessor::render(float time) {
	PostProcessingShader.use();
	PostProcessingShader.setFloat("time", time);
	PostProcessingShader.setInt("confuse", confuse);
	PostProcessingShader.setInt("chaos", chaos);
	PostProcessingShader.setInt("shake", shake);

	glActiveTexture(GL_TEXTURE0);
	m_texture.bind();
	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);
}

void PostProcessor::initRenderData() {
	unsigned int VBO;
	float vertices[]{
		-1.0f, -1.0f, 0.0f, 0.0f,
		 1.0f,  1.0f, 1.0f, 1.0f,
		-1.0f,  1.0f, 0.0f, 1.0f,

		-1.0f, -1.0f, 0.0f, 0.0f,
		 1.0f, -1.0f, 1.0f, 0.0f,
		 1.0f,  1.0f, 1.0f, 1.0f
	};
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindVertexArray(VAO);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}