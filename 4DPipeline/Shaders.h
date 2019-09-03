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
uniform vec3 light_position;

layout (lines_adjacency) in;
layout (triangle_strip, max_vertices = 40) out;

in vec3 v_color[];
out vec3 f_normal;
out vec3 f_color;
out vec3 f_position;

void emit(vec3 normal, vec3 p)
{
	f_normal = normalize(model * vec4(normal, 1.0f)).xyz;
	gl_Position = mvp * vec4(p, 1.0f);
	f_position = gl_Position.xyz;
    EmitVertex();
}

void emit_ground_intersection(vec3 p)
{
	vec3 up  = vec3(0, 0, -1);
	vec3 ground = vec3(0, 0, 0);

	// Project the point to the ground plane
	vec3 direction_normal = normalize(light_position - p);
	float d = -dot(ground, up);
	float v = dot(direction_normal, up);
	float t = -(dot(light_position, up) + d) / v;
	vec3 intersection = light_position + (direction_normal * t);

	//
	f_color = vec3(0.3f, 0.3f, 0.3f);
	f_normal = up;
	gl_Position = mvp * vec4(intersection, 1.0f);
	f_position = gl_Position.xyz;

	EmitVertex();	
}

void main()
{ 
	// Should expose these
	float width = 3;
	vec3 height = vec3(0, 0, 20);

	//
	vec3 up  = vec3(0, 0, -1);
	vec3 down  = vec3(0, 0, 1);

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
    vec3 startOffsetNorm = -normalize((startPrevNorm + startEndNorm) * 0.5f);	

	// The vector in the middel of endStartNorm and endNextNorm
	vec3 endOffsetNorm = -normalize((endStartNorm + endNextNorm) * 0.5f);

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
	emit(up, p3 - height);
	emit(up, p4 - height);	
	emit(up, p1 - height);	
	emit(up, p2 - height);	
	EndPrimitive();


	// Bottom
	//emit(down, p1);
	//emit(down, p2);
	//emit(down, p3);
	//emit(down, p4);
	//EndPrimitive();

	// Left
	emit(endOffsetNorm, p3);
	emit(endOffsetNorm, p3 - height);
	emit(startOffsetNorm, p1);
	emit(startOffsetNorm, p1 - height);
	EndPrimitive();

	// Right
	emit(endOffsetNorm, p4 - height);
	emit(endOffsetNorm, p4);
	emit(startOffsetNorm, p2 - height);
	emit(startOffsetNorm, p2);
	EndPrimitive();
	
	// Shadows	
	//emit_ground_intersection(p1);
	//emit_ground_intersection(p2);
	//emit_ground_intersection(p3);
	//emit_ground_intersection(p4);
	//EndPrimitive();

	emit_ground_intersection(p1 - height);
	emit_ground_intersection(p2 - height);
	emit_ground_intersection(p3 - height);
	emit_ground_intersection(p4 - height);
	EndPrimitive();

	emit_ground_intersection(p3);
	emit_ground_intersection(p3 - height);
	emit_ground_intersection(p1);
	emit_ground_intersection(p1 - height);	
	EndPrimitive();

	emit_ground_intersection(p4 - height);
	emit_ground_intersection(p4);
	emit_ground_intersection(p2 - height);
	emit_ground_intersection(p2);
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
	float diff = max(dot(light_direction, f_normal) , 0.0f);
	vec3 diffuse = diff * light_color;

	//
	vec3 result = (ambient_color + diffuse) * f_color;
	out_color = vec4(result, 1.0f);
} 

)V0G0N";
