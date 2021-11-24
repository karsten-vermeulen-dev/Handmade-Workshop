#include "Design.h"
#include "Input.h"
#include "Screen.h"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_opengl3.h"
#include "imgui/imgui_impl_sdl.h"

//======================================================================================================
bool Design::OnEnter()
{
	m_mainShader = std::make_unique<Shader>();

	if (!m_mainShader->Create("Shaders/Main.vert", "Shaders/Main.frag"))
	{
		return false;
	}

	m_mainShader->BindAttribute("vertexIn");
	m_mainShader->BindAttribute("colorIn");
	m_mainShader->BindAttribute("textureIn");

	m_mainShader->BindUniform("model");
	m_mainShader->BindUniform("view");
	m_mainShader->BindUniform("projection");
	m_mainShader->BindUniform("isTextured");

	//===================================================================

	m_textShader = std::make_unique<Shader>();

	if (!m_textShader->Create("Shaders/Text.vert", "Shaders/Text.frag"))
	{
		return false;
	}

	m_textShader->BindAttribute("vertexIn");
	m_textShader->BindAttribute("colorIn");
	m_textShader->BindAttribute("textureIn");

	m_textShader->BindUniform("model");
	m_textShader->BindUniform("view");
	m_textShader->BindUniform("projection");
	m_textShader->BindUniform("textureImage");

	//===================================================================

	m_lightShader = std::make_unique<Shader>();

	if (!m_lightShader->Create("Shaders/Light.vert", "Shaders/Light.frag"))
	{
		return false;
	}

	m_lightShader->BindAttribute("vertexIn");
	m_lightShader->BindAttribute("colorIn");
	m_lightShader->BindAttribute("textureIn");
	m_lightShader->BindAttribute("normalIn");

	m_lightShader->BindUniform("model");
	m_lightShader->BindUniform("view");
	m_lightShader->BindUniform("projection");
	m_lightShader->BindUniform("normal");

	m_lightShader->BindUniform("isTextured");
	m_lightShader->BindUniform("cameraPosition");

	m_lightShader->BindUniform("light.ambient");
	m_lightShader->BindUniform("light.diffuse");
	m_lightShader->BindUniform("light.specular");
	m_lightShader->BindUniform("light.position");

	m_lightShader->BindUniform("material.ambient");
	m_lightShader->BindUniform("material.diffuse");
	m_lightShader->BindUniform("material.specular");
	m_lightShader->BindUniform("material.shininess");

	//m_lightShader->BindUniform("light.attenuationLinear");
	//m_lightShader->BindUniform("light.attenuationConstant");
	//m_lightShader->BindUniform("light.attenuationQuadratic");

	//TEST CODE to be used later for multiple lights
	/*for (size_t i = 0; i < TOTAL_LIGHTS; i++)
	{
		std::string index = std::to_string(i);

		m_lightShader->BindUniform("lights[" + index + "].ambient");
		m_lightShader->BindUniform("lights[" + index + "].diffuse");
		m_lightShader->BindUniform("lights[" + index + "].specular");
		m_lightShader->BindUniform("lights[" + index + "].position");
		m_lightShader->BindUniform("lights[" + index + "].attenuationConstant");
		m_lightShader->BindUniform("lights[" + index + "].attenuationLinear");
		m_lightShader->BindUniform("lights[" + index + "].attenuationQuadratic");
	}*/

	//===================================================================

	Material::Load("Defaults", "Defaults.mtl");

	//===================================================================

	if (!(Text::Initialize()))
	{
		return false;
	}

	if (!(Audio::Initialize()))
	{
		return false;
	}

	//===================================================================

	m_grid = std::make_unique<Grid>();
	m_grid->GetTransform().SetRotation(45.0f, -30.0f, 0.0f);

	m_axes = std::make_unique<Axes>("Arrow.obj");

	m_topText = std::make_unique<Text>("Quikhand", "Quikhand.ttf", 30);
	m_topText->SetColor(1.0f, 0.0f, 0.196f, 1.0f);
	m_topText->SetString("Handmade Alpha");

	m_bottomText = std::make_unique<Text>("Quikhand");
	m_bottomText->SetColor(0.0f, 0.564f, 1.0f, 1.0f);
	m_bottomText->SetString("Click/Drag the mouse to rotate the grid. Use the mouse wheel to zoom in/out");

	m_axesLabelText = std::make_unique<Text>("Arial", "Arial.ttf", 66);
	m_axesLabelText->SetColor(1.0f, 1.0f, 1.0f, 1.0f);
	m_axesLabelText->SetString("X");

	//For current testing=======================================================

	/*m_text.push_back(Text("Quikhand", "Quickhand.ttf", 10));
	m_text.back().SetColor(1.0f, 0.75f, 0.1f);
	m_text.back().SetString("Text #1");

	m_text.push_back(Text("Quikhand"));
	m_text.back().SetColor(1.0f, 0.75f, 0.1f);
	m_text.back().SetString("Text #2");

	m_text.push_back(Text());
	m_text.back().SetFont("Quickhand");
	m_text.back().SetColor(1.0f, 0.75f, 0.1f);
	m_text.back().SetString("Text #3");*/

	/*Audio::Load(Audio::Type::Music, "Test1", "Armin.wav");
	Audio::Load(Audio::Type::Music, "Test2", "Dance.mp3");
	Audio::Load(Audio::Type::Music, "Test3", "Journeys.ogg");

	m_audio1 = std::make_unique<Audio>(Audio::Type::Music, "Test1");
	m_audio2 = std::make_unique<Audio>(Audio::Type::Music, "Test2");
	m_audio3 = std::make_unique<Audio>(Audio::Type::Music, "Test2");

	m_audio2->Play();*/

	//==========================================================================

	m_light = std::make_unique<Light>(0.0f, 7.5f, 0.0f);

	m_model = std::make_unique<Model>("Teapot", "Teapot.obj", true);
	m_model->GetTransform().SetScale(5.0f, 5.0f, 5.0f);
	//m_model->SetColor(1, 0, 1, 1);

	//m_quad = std::make_unique<Quad>();
	//m_cube = std::make_unique<Cuboid>();
	//m_sphere = std::make_unique<Sphere>(10.0f, 50.0f, 50.0f);

	//=========================================================================

	m_sceneCamera = std::make_unique<FreeCamera>();
	m_sceneCamera->SetVelocity(0.0f);
	m_sceneCamera->SetSensitivity(0.0f);
	m_sceneCamera->GetTransform().SetPosition(0.0f, 0.0f, 50.0f);
	m_sceneCamera->SetBackgroundColor(29U, 29U, 29U);

	//=========================================================================

	Screen::Instance()->SetColor(30U, 30U, 30U);
	
	return true;
}
//======================================================================================================
State* Design::Update(int deltaTime)
{
	Audio::Update();

	if (Input::Instance()->IsXClicked())
	{
		return nullptr;
	}

	//==============================================================================

	auto camPos = m_sceneCamera->GetTransform().GetPosition();
	camPos.z -= (Input::Instance()->GetMouseWheel().y);
	m_sceneCamera->GetTransform().SetPosition(camPos);

	auto mouseMotion = Input::Instance()->GetMouseMotion();
	static glm::vec3 eulerAngles = m_grid->GetTransform().GetEulerAngles();

	if (Input::Instance()->IsLeftButtonClicked())
	{
		eulerAngles.x += -mouseMotion.y;
		eulerAngles.y += mouseMotion.x;
	}

	m_grid->GetTransform().SetRotation(eulerAngles);

	//==============================================================================
	
	for (const auto& object : m_objects)
	{
		if (object->IsActive())
		{
			object->Update(static_cast<GLfloat>(deltaTime));
		}
	}

	return this;
}
//======================================================================================================
bool Design::Render()
{
	auto& mainShader = *m_mainShader.get();
	auto& textShader = *m_textShader.get();
	auto& lightShader = *m_lightShader.get();
	auto& testShader = *m_testShader.get();

	auto resolution = Screen::Instance()->GetResolution();

	auto SetViewport = [](const glm::ivec4& viewport, const glm::uvec4& color)
	{
		Screen::Instance()->SetViewport(viewport.x, viewport.y, viewport.z, viewport.w);
		Screen::Instance()->SetColor(color);
		Screen::Instance()->Refresh();
	};

	const auto MINOR_WIDTH = static_cast<GLsizei>(resolution.x * 0.20f);
	const auto MAJOR_WIDTH = static_cast<GLsizei>(resolution.x * 0.80f);
	const auto MINOR_HEIGHT = static_cast<GLsizei>(resolution.y * 0.20f);
	const auto MAJOR_HEIGHT = static_cast<GLsizei>(resolution.y * 0.80f);

	//Console viewport
	SetViewport(glm::ivec4(0, 0, MAJOR_WIDTH, MINOR_HEIGHT), 
		glm::uvec4(255U, 200U, 0U, 1U));
	
	//Properties viewport
	SetViewport(glm::ivec4(MAJOR_WIDTH, 0, MINOR_WIDTH, resolution.y),
		glm::uvec4(0U, 144U, 255U, 1U));

	//Scene viewport
	m_sceneCamera->SetViewport(0, MINOR_HEIGHT, MAJOR_WIDTH, MAJOR_HEIGHT);
	m_sceneCamera->CreatePerspView();
	
	mainShader.Use();
	
	m_sceneCamera->Update(16.0f);
	m_sceneCamera->SendToShader(mainShader);

	//==============================================================================

	m_grid->Render(mainShader);
	lightShader.Use();
	lightShader.SendData("cameraPosition", m_sceneCamera->GetTransform().GetPosition());
	m_model->Render(lightShader);
	m_light->SendToShader(lightShader);
	m_sceneCamera->SendToShader(lightShader);

	m_model->GetTransform().SetRotation(m_grid->GetTransform().GetRotation());
	m_model->GetTransform().SetPosition(m_grid->GetTransform().GetPosition());

	//==============================================================================
	//ImGUI UI (WIP)
	//==============================================================================
	// Start the Dear ImGui frame
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame();
	ImGui::NewFrame();

	//This makes sure the mouse has no effect on sizing/dragging the window
	ImGui::SetWindowFocus(nullptr);

	ImGui::Begin("Output console");
	
	const auto PADDING = 2.0f;
	auto windowPos = ImVec2(PADDING, 
		static_cast<float>(MAJOR_HEIGHT + PADDING + 1.0f));
	auto windowSize = ImVec2(static_cast<float>(MAJOR_WIDTH - PADDING * 2.0f), 
		static_cast<float>(MINOR_HEIGHT - PADDING * 2.0f));

	ImGui::SetWindowPos("Output console", windowPos);
	ImGui::SetWindowSize("Output console", windowSize);
	
	ImGui::Text("This is where all your debug data will live...");  
	
	ImGui::End();

	ImGui::Begin("Properties");

	windowPos = ImVec2(static_cast<float>(MAJOR_WIDTH + PADDING), PADDING);
	windowSize = ImVec2(static_cast<float>(MINOR_WIDTH - PADDING * 2.0f), 
		static_cast<float>(resolution.y - PADDING * 2.0f));

	ImGui::SetWindowPos("Properties", windowPos);
	ImGui::SetWindowSize("Properties", windowSize);
	
	ImGui::End();

	ImGui::Begin("Cords&Colors");
	ImGui::Button("Reset");
	ImGui::Text("Change Coordinates");
	
	m_model->GetTransform().SetPosition(X, Y, Z);
	ImGui::SliderFloat("Change X axis", &X, -100.0f, 100.0f);
	ImGui::SliderFloat("Change Y axis", &Y, -100.0f, 100.0f);
	ImGui::SliderFloat("Change Z Axis", &Z, -100.0f, 100.0f);


	bool showWindow = false;
	ImGui::Checkbox("Apply color in real-time", &showWindow);
	//ImVec4 color = ImVec4(R, G, B, A); <--This could be an issue
	static float my_color[] = { R,G,B,A };
	ImGui::ColorEdit3("clear color", my_color);
	ImGui::End();

	ImGui::Begin("Output");
	ImGui::TextColored(ImVec4(0, 2, 0, 1), "Changes made");
	ImGui::BeginChild("Scrolling");
	ImGui::Text("display color changes here somehow");
	ImGui::EndChild();
	ImGui::End();

	//bool showWindow = false;
	//ImGui::Checkbox("Check me", &showWindow);

	//ImVec4 color = ImVec4(0.0f, 0.0f, 0.0f, 1.0f);
	//ImGui::ColorEdit3("clear color", (float*)&color);

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	return true;
}
//======================================================================================================
void Design::OnExit()
{
	m_objects.clear();
	Audio::Shutdown();
	Text::Shutdown();
}