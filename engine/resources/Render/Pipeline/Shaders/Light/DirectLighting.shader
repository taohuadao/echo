<?xml version="1.0" encoding="utf-8"?>
<res class="ShaderProgram" Type="glsl" Domain="Lighting" CullMode="CULL_BACK" BlendMode="Opaque" Uniforms.GBuffer_Normal="Engine://Render/Pipeline/Framebuffer/GBuffer/GBufferNormal.rt" Uniforms.GBuffer_Position="Engine://Render/Pipeline/Framebuffer/GBuffer/GBufferPosition.rt" Uniforms.ShadowDepthTex="Engine://Render/Pipeline/Framebuffer/ShadowDepth/ShadowDepthColorA.rt">
	<property name="VertexShader"><![CDATA[#version 450

struct Position
{
    vec3 local;
    vec3 world;
    vec3 view;
};

layout(binding = 0, std140) uniform UBO
{
    mat4 u_WorldMatrix;
    mat4 u_ViewProjMatrix;
} vs_ubo;

layout(location = 0) in vec3 a_Position;
layout(location = 0) out Position v_Position;
layout(location = 4) out vec3 v_NormalLocal;
layout(location = 1) in vec3 a_Normal;
layout(location = 3) out vec3 v_Normal;
layout(location = 6) out vec4 v_Color;
layout(location = 3) in vec4 a_Color;
layout(location = 7) out vec2 v_UV;
layout(location = 4) in vec2 a_UV;

void main()
{
    vec4 worldPosition = vs_ubo.u_WorldMatrix * vec4(a_Position, 1.0);
    vec4 clipPosition = vs_ubo.u_ViewProjMatrix * worldPosition;
    v_Position.local = a_Position;
    v_Position.world = worldPosition.xyz;
    v_Position.view = ((clipPosition.xyz / vec3(clipPosition.w)) + vec3(1.0)) * 0.5;
    gl_Position = clipPosition;
    v_NormalLocal = a_Normal;
    v_Normal = normalize(vec3((vs_ubo.u_WorldMatrix * vec4(a_Normal, 0.0)).xyz));
    v_Color = a_Color;
    v_UV = a_UV;
}

]]></property>
	<property name="FragmentShader"><![CDATA[#version 450

struct Position
{
    vec3 local;
    vec3 world;
    vec3 view;
};

layout(binding = 1, std140) uniform UBO
{
    mat4 u_ShadowCameraProjMatrix;
    mat4 u_ShadowCameraViewProjMatrix;
    vec3 u_ShadowCameraPosition;
    vec3 u_ShadowCameraDirection;
    float u_ShadowCameraNear;
    float u_ShadowCameraFar;
} fs_ubo;

layout(binding = 2) uniform sampler2D GBuffer_Normal;
layout(binding = 4) uniform sampler2D GBuffer_Position;
layout(binding = 3) uniform sampler2D ShadowDepthTex;

layout(location = 7) in vec2 v_UV;
layout(location = 3) in vec3 v_Normal;
layout(location = 6) in vec4 v_Color;
layout(location = 0) out vec4 o_FragDiffuse;
layout(location = 1) out vec4 o_FragSpecular;
layout(location = 0) in Position v_Position;
layout(location = 4) in vec3 v_NormalLocal;

vec3 Diffuse(vec3 InNormal, vec3 InLightDir, vec4 InLightColor)
{
    vec3 Normal = (InNormal * 2.0) - vec3(1.0);
    return InLightColor.xyz * dot(InNormal, -InLightDir);
}

float ShadowMapCalculation(sampler2D texShdowDepth, vec3 worldPosition)
{
    vec4 clip = fs_ubo.u_ShadowCameraViewProjMatrix * vec4(worldPosition, 1.0);
    vec3 ndc = clip.xyz / vec3(clip.w);
    vec3 win = (ndc / vec3(2.0)) + vec3(0.5);
    float depthInShadowMap = texture(texShdowDepth, win.xy).x;
    float depthCurrent = win.z;
    float n = fs_ubo.u_ShadowCameraNear;
    float f = fs_ubo.u_ShadowCameraFar;
    float depthInShadowMapLinear = (depthInShadowMap * (f - n)) + n;
    float depthCurrentLinear = (depthCurrent * (f - n)) + n;
    float lightingBias = 0.1500000059604644775390625;
    float _distance = depthCurrentLinear - depthInShadowMapLinear;
    float lighting = exp(((-3.5) * _distance) + lightingBias);
    return clamp(lighting, 0.0, 1.0);
}

void main()
{
    vec4 Color_627_Value = vec4(0.0, 0.0, 0.0, 1.0);
    vec4 GBuffer_Normal_Color = texture(GBuffer_Normal, v_UV);
    vec4 GBuffer_Position_Color = texture(GBuffer_Position, v_UV);
    vec3 param = GBuffer_Normal_Color.xyz;
    vec3 param_1 = v_Normal;
    vec4 param_2 = v_Color;
    vec3 GLSL_569 = Diffuse(param, param_1, param_2);
    vec3 param_3 = GBuffer_Position_Color.xyz;
    float ShadowMapCalculation_288 = ShadowMapCalculation(ShadowDepthTex, param_3);
    vec3 Multiplication_356 = GLSL_569 * ShadowMapCalculation_288;
    vec3 _Diffuse = Multiplication_356;
    vec3 _Specular = Color_627_Value.xyz;
    o_FragDiffuse = vec4(_Diffuse, 1.0);
    o_FragSpecular = vec4(_Specular, 1.0);
}

]]></property>
	<property name="Graph"><![CDATA[{
    "connections": [
        {
            "in_id": "{c34ddd04-fba4-4882-804d-7e633f55e4dc}",
            "in_index": 0,
            "out_id": "{26bfaf9c-ed54-4a30-ac06-d8e13b74410e}",
            "out_index": 0
        },
        {
            "in_id": "{5dbf7943-a2c1-470c-8dec-7aa0e0817f98}",
            "in_index": 2,
            "out_id": "{7028fed8-7e12-4907-aa83-5d7b810b388e}",
            "out_index": 0
        },
        {
            "in_id": "{c34ddd04-fba4-4882-804d-7e633f55e4dc}",
            "in_index": 1,
            "out_id": "{b06a016b-ddaf-45fe-8b7f-8ffeffce3549}",
            "out_index": 1
        },
        {
            "in_id": "{920e9e49-c656-4d39-91aa-3ded63350483}",
            "in_index": 1,
            "out_id": "{e2eb8b65-65fd-425d-93c8-944c74cd8c75}",
            "out_index": 0
        },
        {
            "in_id": "{5dbf7943-a2c1-470c-8dec-7aa0e0817f98}",
            "in_index": 1,
            "out_id": "{be8c8bd3-7694-4adc-8762-9f2645122d0f}",
            "out_index": 0
        },
        {
            "in_id": "{5dbf7943-a2c1-470c-8dec-7aa0e0817f98}",
            "in_index": 0,
            "out_id": "{1b18812f-5203-4858-9e11-64f3a3df9a09}",
            "out_index": 1
        },
        {
            "converter": {
                "in": {
                    "id": "any",
                    "name": "B"
                },
                "out": {
                    "id": "float",
                    "name": "float"
                }
            },
            "in_id": "{e80d3feb-2438-4b69-ae29-23ed6327748b}",
            "in_index": 1,
            "out_id": "{c34ddd04-fba4-4882-804d-7e633f55e4dc}",
            "out_index": 0
        },
        {
            "converter": {
                "in": {
                    "id": "any",
                    "name": "A"
                },
                "out": {
                    "id": "vec3",
                    "name": "vec3"
                }
            },
            "in_id": "{e80d3feb-2438-4b69-ae29-23ed6327748b}",
            "in_index": 0,
            "out_id": "{5dbf7943-a2c1-470c-8dec-7aa0e0817f98}",
            "out_index": 0
        },
        {
            "in_id": "{920e9e49-c656-4d39-91aa-3ded63350483}",
            "in_index": 0,
            "out_id": "{e80d3feb-2438-4b69-ae29-23ed6327748b}",
            "out_index": 0
        }
    ],
    "nodes": [
        {
            "id": "{e2eb8b65-65fd-425d-93c8-944c74cd8c75}",
            "model": {
                "Color": "0 0 0 1 ",
                "Uniform": "false",
                "Variable": "Color_627",
                "name": "Color"
            },
            "position": {
                "x": 3,
                "y": 518
            }
        },
        {
            "id": "{920e9e49-c656-4d39-91aa-3ded63350483}",
            "model": {
                "Variable": "ShaderTemplate_624",
                "name": "ShaderTemplateLighting"
            },
            "position": {
                "x": 341,
                "y": 433
            }
        },
        {
            "id": "{1b18812f-5203-4858-9e11-64f3a3df9a09}",
            "model": {
                "Atla": "false",
                "Texture": "Engine://Render/Pipeline/Framebuffer/GBuffer/GBufferNormal.rt",
                "Type": "General",
                "Variable": "GBuffer_Normal",
                "name": "Texture"
            },
            "position": {
                "x": -761,
                "y": 24
            }
        },
        {
            "id": "{be8c8bd3-7694-4adc-8762-9f2645122d0f}",
            "model": {
                "Attribute": "direction",
                "Variable": "DirectionLight_570",
                "name": "DirectionLight"
            },
            "position": {
                "x": -810,
                "y": 168
            }
        },
        {
            "id": "{5dbf7943-a2c1-470c-8dec-7aa0e0817f98}",
            "model": {
                "Code": "vec3 Diffuse(vec3 InNormal, vec3 InLightDir, vec4 InLightColor)\n{\n\tvec3 Normal = InNormal * 2.0 - 1.0;\n\treturn InLightColor.rgb * dot(InNormal, -InLightDir);\n}",
                "FunctionName": "Diffuse",
                "Parameters": "vec3 InNormal, vec3 InLightDir, vec4 InLightColor",
                "ReturnType": "vec3",
                "Variable": "GLSL_569",
                "name": "GLSL"
            },
            "position": {
                "x": -371,
                "y": 154
            }
        },
        {
            "id": "{7028fed8-7e12-4907-aa83-5d7b810b388e}",
            "model": {
                "Attribute": "color",
                "Variable": "DirectionLight_571",
                "name": "DirectionLight"
            },
            "position": {
                "x": -812,
                "y": 254
            }
        },
        {
            "id": "{26bfaf9c-ed54-4a30-ac06-d8e13b74410e}",
            "model": {
                "Atla": "false",
                "Texture": "Engine://Render/Pipeline/Framebuffer/ShadowDepth/ShadowDepthColorA.rt",
                "Type": "General",
                "Variable": "ShadowDepthTex",
                "name": "Texture"
            },
            "position": {
                "x": -764,
                "y": 366
            }
        },
        {
            "id": "{b06a016b-ddaf-45fe-8b7f-8ffeffce3549}",
            "model": {
                "Atla": "false",
                "Texture": "Engine://Render/Pipeline/Framebuffer/GBuffer/GBufferPosition.rt",
                "Type": "General",
                "Variable": "GBuffer_Position",
                "name": "Texture"
            },
            "position": {
                "x": -766,
                "y": 518
            }
        },
        {
            "id": "{c34ddd04-fba4-4882-804d-7e633f55e4dc}",
            "model": {
                "Code": "float ShadowMapCalculation(sampler2D texShdowDepth, vec3 worldPosition)\n{\n\thighp vec4 clip = fs_ubo.u_ShadowCameraViewProjMatrix * vec4(worldPosition, 1.0);\n\thighp vec3 ndc = clip.xyz / clip.w;\n\thighp vec3 win = ndc / 2.0 + 0.5;\n\t\t\n\thighp float depthInShadowMap = texture(texShdowDepth, win.xy).r;\n\thighp float depthCurrent = win.z;\n\n\thighp float n = fs_ubo.u_ShadowCameraNear;\n\thighp float f = fs_ubo.u_ShadowCameraFar;\n\n\t// Orthographic Projection\n\thighp float depthInShadowMapLinear = depthInShadowMap * (f - n) + n;\n\thighp float depthCurrentLinear = depthCurrent * (f - n) + n;\n\n\thighp float lightingBias = 0.15;\n\thighp float distance = depthCurrentLinear-depthInShadowMapLinear;\n\thighp float lighting = exp(-3.5 * distance + lightingBias);\n\n\treturn clamp(lighting, 0.0, 1.0);\n}",
                "FunctionName": "ShadowMapCalculation",
                "Parameters": "sampler2D texShdowDepth, vec3 worldPosition",
                "ReturnType": "float",
                "Variable": "ShadowMapCalculation_288",
                "name": "ShadowMapCalculation"
            },
            "position": {
                "x": -384,
                "y": 432
            }
        },
        {
            "id": "{e80d3feb-2438-4b69-ae29-23ed6327748b}",
            "model": {
                "Variable": "Multiplication_356",
                "name": "Multiplication"
            },
            "position": {
                "x": -31,
                "y": 268
            }
        }
    ]
}
]]></property>
	<property name="DepthStencilState">
		<obj class="DepthStencilState" DepthEnable="true" WriteDepth="true" />
	</property>
</res>
