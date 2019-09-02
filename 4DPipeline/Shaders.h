#pragma once

static const char* vertex_shader_text = R"V0G0N(
#version 400

in vec3 position;
in vec3 color;

out vec3 v_color;
void main() 
{
    gl_Position = vec4(position, 1.0f);
    v_color = color;
}
)V0G0N";

static const char* geometry_shader_text = R"V0G0N(				
#version 400

uniform mat4 mvp;
uniform mat4 model;
uniform mat4 view;

layout (lines_adjacency) in;
layout (triangle_strip, max_vertices = 20) out;

in vec3 v_color[];
out vec3 f_normal;
out vec3 f_color;
out vec3 f_position;

void Emit(vec3 normal, vec3 p)
{
	f_normal = normalize(model * vec4(normal, 1.0f)).xyz;
	gl_Position = mvp * vec4(p, 1.0f);
	f_position = gl_Position.xyz;
    EmitVertex();
}

void main()
{ 
	// Should expose these
	float width = 2.0f;
	float height = -40;
	vec3 half_height = vec3(0, 0, height / 2.0f);

	//
	vec3 up  = vec3(0, 0, 1);
	vec3 down  = vec3(0, 0, -1);

    vec3 prev = gl_in[0].gl_Position.xyz;
    vec3 start = gl_in[1].gl_Position.xyz;
    vec3 end = gl_in[2].gl_Position.xyz;
    vec3 next = gl_in[3].gl_Position.xyz;
   	
	// Segment's own normals
	vec3 endStartNorm = normalize(end - start);
	vec3 startEndNorm = normalize(start - end);

	// Normals between segment and adjacent points
	vec3 endNextNorm = normalize(end - next);
	vec3 startPrevNorm = normalize(start - prev);

	//
	vec3 leftVector = cross(endStartNorm, up);
    
	// The vector in the middel of startPrev and startEnd	
    vec3 startOffsetNorm = normalize((startPrevNorm + startEndNorm) * 0.5f);	

	// The vector in the middel of endStartNorm and endNextNorm
	vec3 endOffsetNorm = normalize((endStartNorm + endNextNorm) * 0.5f);

	// Half the width on each side
    vec3 startOffset = startOffsetNorm * width * 0.5f;
    vec3 endOffset = endOffsetNorm * width * 0.5f;

	// Flip the offset if it's > 90 away from the leftVector
	// This keeps the offset vector always pointing to the same side of the path
    startOffset *= sign(dot(startOffsetNorm, leftVector));
    endOffset *= sign(dot(endOffsetNorm, leftVector));

	//
	f_color = v_color[0];

	// Calculate the 4 corners
	vec3 p1 = start + startOffset;
	vec3 p2 = start - startOffset;
	vec3 p3 = end + endOffset;
	vec3 p4 = end - endOffset;

	// Top
	Emit(up, p3 - half_height);
	Emit(up, p4 - half_height);	
	Emit(up, p1 - half_height);	
	Emit(up, p2 - half_height);	
	EndPrimitive();

	// Bottom
	Emit(down, p1 + half_height);
	Emit(down, p2 + half_height);
	Emit(down, p3 + half_height);
	Emit(down, p4 + half_height);
	EndPrimitive();

	// Left
	Emit(endOffset, p3 + half_height);
	Emit(endOffset, p3 - half_height);
	Emit(startOffset, p1 + half_height);
	Emit(startOffset, p1 - half_height);
	EndPrimitive();

	// Right
	Emit(endOffset, p4 - half_height);
	Emit(endOffset, p4 + half_height);
	Emit(startOffset, p2 - half_height);
	Emit(startOffset, p2 + half_height);
	EndPrimitive();
}

)V0G0N";

static const char* fragment_shader_text = R"V0G0N(
#version 400

uniform vec3 light_position;

in vec3 f_color;
in vec3 f_normal;  
in vec3 f_position;
out vec4 out_color;

void main()
{
	// Could expose these
	vec3 ambient_color = vec3(0.4f, 0.4f, 0.4f);
	vec3 light_color = vec3(1.0f, 1.0f, 1.0f);

	//
	vec3 light_direction = normalize(light_position - f_position);  

	//
	float diff = max(dot(f_normal, light_direction), 0.0f);
	vec3 diffuse = diff * light_color;

	//
	vec3 result = (ambient_color + diffuse) * f_color;
	out_color = vec4(result, 1.0);
} 

)V0G0N";
