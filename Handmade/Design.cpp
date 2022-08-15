#include <functional>

#include "imgui/imgui.h"
#include "imgui/imgui_impl_opengl3.h"
#include "imgui/imgui_impl_sdl.h"

#include "BoxCollider.h"
#include "Design.h"
#include "Input.h"
#include "Screen.h"
#include "Utility.h"

//======================================================================================================
Design::Design() {}
//======================================================================================================
bool Design::OnEnter()
{
	//TODO - Use a color picker to change this
	Screen::Instance()->SetColor(30U, 30U, 30U);

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

	m_consoleLog.push_front("'Main.vert' and 'Main.frag' shaders created and compiled.");

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

	m_consoleLog.push_front("'Text.vert' and 'Text.frag' shaders created and compiled.");

	//===================================================================
	//TODO - There is a bug with the lighting shaders

	m_lightShader = std::make_unique<Shader>();

	if (!m_lightShader->Create("Shaders/Light.vert", "Shaders/Light.frag"))
	{
		return false;
	}

	/*m_lightShader->BindAttribute("vertexIn");
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
	m_lightShader->BindUniform("material.shininess");*/

	//m_lightShader->BindUniform("light.attenuationLinear");
	//m_lightShader->BindUniform("light.attenuationConstant");
	//m_lightShader->BindUniform("light.attenuationQuadratic");

	//m_consoleLog.push_front("'Light.vert' and 'Light.frag' shaders created and compiled.");

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

	//Find a different way to load standard default materials
	//Material::Load("Defaults", "Defaults.mtl");
	m_consoleLog.push_front("Default materials loaded.");

	//===================================================================

	//TODO - Move these to a global messaging system so 
	//that the 'App' class can register these messages
	//m_consoleLog.push_front("Text sub-system initialized.");
	//m_consoleLog.push_front("Audio sub-system initialized.");

	//=========================================================================

	ImGui::GetIO().Fonts->AddFontFromFileTTF("Assets/Fonts/Quikhand.ttf", FONT_SIZE);
	ImGui::GetIO().Fonts->Build();

	//WIP======================================================================
	//Adding objects to the scene
	//========================================================================= 

	m_grid = std::make_unique<Grid>("Scene");
	m_grid->GetTransform().SetRotation(45.0f, -30.0f, 0.0f);

	m_sceneCamera = std::make_unique<FreeCamera>("Main_cam");
	m_sceneCamera->SetVelocity(0.0f);
	m_sceneCamera->SetSensitivity(0.0f);
	m_sceneCamera->GetTransform().SetPosition(0.0f, 0.0f, 50.0f);
	m_sceneCamera->SetBackgroundColor(29U, 29U, 29U);
	m_consoleLog.push_front("Scene camera created.");

	//=========================================================================

	/*m_topText = std::make_unique<Text>("Quikhand", "Quikhand.ttf", 30);
	m_topText->SetColor(1.0f, 0.0f, 0.196f, 1.0f);
	m_topText->SetString("Handmade Alpha");*/

	/*m_bottomText = std::make_unique<Text>("Quikhand");
	m_bottomText->SetColor(0.0f, 0.564f, 1.0f, 1.0f);
	m_bottomText->SetString("Click/Drag the mouse to rotate the grid. Use the mouse wheel to zoom in/out");*/

	/*m_axesLabelText = std::make_unique<Text>("Arial", "Arial.ttf", 66);
	m_axesLabelText->SetColor(1.0f, 1.0f, 1.0f, 1.0f);
	m_axesLabelText->SetString("X");*/

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

	//m_light = std::make_unique<Light>(0.0f, 7.5f, 0.0f);

	//m_model = std::make_unique<Model>("Teapot", "Teapot.obj", true);
	//m_model->GetTransform().SetScale(5.0f, 5.0f, 5.0f);
	//m_model->SetColor(1, 0, 1, 1);

	//m_sphere = std::make_unique<Sphere>(10.0f, 50.0f, 50.0f);

	return true;
}
//======================================================================================================
State* Design::Update(int deltaTime)
{
	Audio::Update();

	if (Input::Instance()->IsXClicked() || m_isStateComplete)
	{
		return nullptr;
	}

	//==============================================================================

	BoxCollider sceneBox;
	auto dimension = m_sceneCamera->GetResolution();

	sceneBox.SetPosition(m_minorWidth + (dimension.x * 0.5f), dimension.y * 0.5f, 0.0f);
	sceneBox.SetDimension(static_cast<GLfloat>(dimension.x), static_cast<GLfloat>(dimension.y), 0.0f);
	sceneBox.Update();

	BoxCollider mouseBox;
	auto mousePosition = Input::Instance()->GetMousePosition();
	mouseBox.SetPosition(static_cast<GLfloat>(mousePosition.x), static_cast<GLfloat>(mousePosition.y), 0.0f);
	mouseBox.SetDimension(1.0f, 1.0f, 0.0f);
	mouseBox.Update();

	//================================================================================

	if (sceneBox.IsColliding(mouseBox))
	{
		//Zoom
		auto camPos = m_sceneCamera->GetTransform().GetPosition();
		camPos.z -= (Input::Instance()->GetMouseWheel().y);
		m_sceneCamera->GetTransform().SetPosition(camPos);

		//Rotate grid
		if (Input::Instance()->IsLeftButtonClicked())
		{
			auto mouseMotion = Input::Instance()->GetMouseMotion();
			m_sceneRotation.x += -mouseMotion.y;
			m_sceneRotation.y += mouseMotion.x;
			m_grid->GetTransform().SetRotation(m_sceneRotation);
		}
	}

	//==============================================================================

	for (const auto& object : m_objects)
	{
		if (object->IsActive())
		{
			object->Update(static_cast<GLfloat>(deltaTime));
		}
	}

	//==============================================================================

	m_resolution = Screen::Instance()->GetResolution();
	m_minorWidth = static_cast<GLsizei>(m_resolution.x * MINOR);
	m_majorWidth = static_cast<GLsizei>(m_resolution.x * MAJOR);
	m_minorHeight = static_cast<GLsizei>(m_resolution.y * MINOR);
	m_majorHeight = static_cast<GLsizei>(m_resolution.y * MAJOR);

	return this;
}
//======================================================================================================
bool Design::Render()
{
	auto& mainShader = *m_mainShader.get();
	auto& textShader = *m_textShader.get();
	auto& lightShader = *m_lightShader.get();
	auto& testShader = *m_testShader.get();

	auto SetViewport = [](const glm::ivec4& viewport, const glm::uvec4& color)
	{
		Screen::Instance()->SetViewport(viewport.x, viewport.y, viewport.z, viewport.w);
		Screen::Instance()->SetColor(color);
		Screen::Instance()->Refresh();
	};

	//Hierarchy viewport
	SetViewport(glm::ivec4(0, m_minorHeight, m_minorWidth, m_resolution.y - m_minorHeight - MENU_BAR_HEIGHT),
		glm::uvec4(0U, 144U, 255U, 1U));

	//Console viewport
	SetViewport(glm::ivec4(0, 0, m_majorWidth, m_minorHeight),
		glm::uvec4(0U, 144U, 255U, 1U));

	//Properties viewport
	SetViewport(glm::ivec4(m_majorWidth, 0, m_minorWidth, m_resolution.y),
		glm::uvec4(0U, 144U, 255U, 1U));

	//Scene viewport
	m_sceneCamera->SetViewport(m_minorWidth, m_minorHeight, m_resolution.x - (m_minorWidth * 2), m_majorHeight);
	m_sceneCamera->CreatePerspView();

	mainShader.Use();

	m_sceneCamera->Update(16.0f);
	m_sceneCamera->SendToShader(mainShader);

	//==============================================================================

	m_grid->Render(mainShader);

	for (const auto& object : m_objects)
	{
		object->Render(mainShader);
	}

	/*lightShader.Use();
	lightShader.SendData("cameraPosition", m_sceneCamera->GetTransform().GetPosition());

	m_light->SendToShader(lightShader);
	m_light->Render(lightShader);
	m_sceneCamera->SendToShader(lightShader);*/

	//m_axes->GetTransform().SetRotation(m_grid->GetTransform().GetRotation());
	//m_axes->Render(lightShader);

	//m_cube->GetTransform().SetRotation(m_grid->GetTransform().GetRotation());
	//m_cube->Render(lightShader);

	//m_model->GetTransform().SetRotation(m_grid->GetTransform().GetRotation());
	//m_model->Render(lightShader);

	//==============================================================================
	//Text rendering & UI
	//==============================================================================

	/*textShader.Use();

	m_sceneCamera->CreateOrthoView();
	m_sceneCamera->Update(16.0f);
	m_sceneCamera->SendToShader(textShader);

	auto labelPosition = m_sceneCamera->ConvertWorldToScreen(m_axes->GetArrowTipPositionX());
	m_axesLabelText->IsFirstLetterCentered(true);
	m_axesLabelText->GetTransform().SetPosition(labelPosition.x, labelPosition.y, 0.0f);
	m_axesLabelText->SetString("X");
	m_axesLabelText->SendToShader(textShader);
	m_axesLabelText->Render(textShader);*/

	//labelPosition = m_mainCamera->ConvertWorldToScreen(m_axes->GetArrowTipPositionY());
	//m_axesLabelText->GetTransform().SetPosition(labelPosition.x, labelPosition.y, 0.0f);
	//m_axesLabelText->SendToShader(textShader);
	//m_axesLabelText->SetString("Y");
	//m_axesLabelText->Render(textShader);

	//labelPosition = m_mainCamera->ConvertWorldToScreen(m_axes->GetArrowTipPositionZ());
	//m_axesLabelText->GetTransform().SetPosition(labelPosition.x, labelPosition.y, 0.0f);
	//m_axesLabelText->SendToShader(textShader);
	//m_axesLabelText->SetString("Z");
	//m_axesLabelText->Render(textShader);

	//For current testing
	/*auto count = 0;

	for (auto& text : m_text)
	{
		text.GetTransform().SetPosition(10.0f, (resolution.y - 50.0f - count * 100.0f), 0.0f);
		text.SendToShader(textShader);
		text.Render(textShader);
		count++;
	}*/

	//m_quad->GetTransform().SetRotation(m_grid->GetTransform().GetRotation());
	//m_quad->Render(*Shader::Instance());

	//m_sphere->GetTransform().SetRotation(m_grid->GetTransform().GetRotation());
	//m_sphere->Render(*Shader::Instance());

	//TEST CODE to be used later on
	/*m_UICamera->SetOrthoView();
	m_UICamera->Update();

	glm::vec2 pixels = m_mainCamera->ConvertWorldToScreen(m_axes->GetArrowTipPositionX());
	m_labelX->GetTransform().SetPosition(pixels.x, pixels.y, 0.0f);
	m_labelX->Render();

	pixels = m_mainCamera->ConvertWorldToScreen(m_axes->GetArrowTipPositionY());
	m_labelY->GetTransform().SetPosition(pixels.x, pixels.y, 0.0f);
	m_labelY->Render();

	pixels = m_mainCamera->ConvertWorldToScreen(m_axes->GetArrowTipPositionZ());
	m_labelZ->GetTransform().SetPosition(pixels.x, pixels.y, 0.0f);
	m_labelZ->Render();*/

	/*for (const auto& object : m_objects)
	{
		if (object->IsVisible())
		{
			object->Render(lightShader);
		}
	}*/

	//==============================================================================
	//ImGUI UI (WIP)
	//==============================================================================

	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame();
	ImGui::NewFrame();

	RenderMenu();
	RenderConsoleWindow();
	RenderHierarchyWindow();
	RenderPropertiesWindow();

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
//======================================================================================================
void Design::RenderMenu()
{
	if (ImGui::BeginMainMenuBar())
	{
		//Do not use else-if statements here otherwise
		//the menu titles will not render correctly
		if (ImGui::BeginMenu("Scene"))
		{
			if (ImGui::MenuItem("New", nullptr, nullptr))
			{
				//Handle menu item...
			}

			if (ImGui::MenuItem("Load...", nullptr, nullptr))
			{
				//Handle menu item...
			}

			if (ImGui::MenuItem("Save", nullptr, nullptr))
			{
				//Handle menu item...
			}

			if (ImGui::MenuItem("Save As...", nullptr, nullptr))
			{
				//Handle menu item...
			}

			if (ImGui::MenuItem("Reset", nullptr, nullptr))
			{
				//Handle menu item...
			}

			ImGui::Separator();

			if (ImGui::MenuItem("Exit", nullptr, nullptr))
			{
				m_isStateComplete = true;
			}

			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Objects"))
		{
			if (ImGui::MenuItem("Quad", nullptr, nullptr))
			{
				m_objects.emplace_back(std::make_unique<Quad>("Quad_" + std::to_string(Quad::GetTotalQuads() + 1)));
				m_grid->AddChild(m_objects.back().get());
			}

			if (ImGui::MenuItem("Circle", nullptr, nullptr))
			{
				m_objects.emplace_back(std::make_unique<Circle>("Circle_" + std::to_string(Circle::GetTotalCircles() + 1)));
				m_grid->AddChild(m_objects.back().get());
			}

			if (ImGui::MenuItem("Cuboid", nullptr, nullptr))
			{
				m_objects.emplace_back(std::make_unique<Cuboid>("Cube_" + std::to_string(Cuboid::GetTotalCuboids() + 1)));
				m_grid->AddChild(m_objects.back().get());
			}

			if (ImGui::MenuItem("Sphere", nullptr, nullptr))
			{
				m_objects.emplace_back(std::make_unique<Sphere>("Sphere_" + std::to_string(Sphere::GetTotalSpheres() + 1)));
				m_grid->AddChild(m_objects.back().get());
			}

			ImGui::Separator();

			if (ImGui::MenuItem("Text", nullptr, nullptr))
			{
				//Handle menu item...
			}

			if (ImGui::MenuItem("Tile", nullptr, nullptr))
			{
				//Handle menu item...
			}

			if (ImGui::MenuItem("Light", nullptr, nullptr))
			{
				//Handle menu item...
			}

			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Custom"))
		{
			if (ImGui::MenuItem("Model...", nullptr, nullptr))
			{
				//Handle menu item...
			}

			if (ImGui::MenuItem("Shader...", nullptr, nullptr))
			{
				//Handle menu item...
			}

			if (ImGui::MenuItem("Audio...", nullptr, nullptr))
			{
				//Handle menu item...
			}

			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Help"))
		{
			if (ImGui::MenuItem("Tutorial", nullptr, nullptr))
			{
				//Handle menu item...
			}

			if (ImGui::MenuItem("About...", nullptr, nullptr))
			{
				//Handle menu item...
			}

			ImGui::EndMenu();
		}

		ImGui::EndMainMenuBar();
	}
}
//======================================================================================================
void Design::RenderConsoleWindow()
{
	ImGui::Begin("Output console", nullptr,
		ImGuiWindowFlags_::ImGuiWindowFlags_NoTitleBar |
		ImGuiWindowFlags_::ImGuiWindowFlags_NoResize |
		ImGuiWindowFlags_::ImGuiWindowFlags_NoMove |
		ImGuiWindowFlags_::ImGuiWindowFlags_NoCollapse);

	auto windowPos = ImVec2(UI_PADDING,
		static_cast<float>(m_majorHeight + UI_PADDING + 1.0f));
	auto windowSize = ImVec2(static_cast<float>(m_majorWidth - UI_PADDING),
		static_cast<float>(m_minorHeight - UI_PADDING * 2.0f));

	ImGui::SetWindowPos("Output console", windowPos);
	ImGui::SetWindowSize("Output console", windowSize);

	if (m_consoleLog.size() > MAX_CONSOLE_LOG_SIZE)
	{
		m_consoleLog.pop_back();
	}

	for (auto& log : m_consoleLog)
	{
		ImGui::Text(log.c_str());
	}

	ImGui::End();
}
//======================================================================================================
void Design::RenderHierarchyWindow()
{
	ImGui::Begin("Hierarchy", nullptr,
		ImGuiWindowFlags_::ImGuiWindowFlags_NoTitleBar |
		ImGuiWindowFlags_::ImGuiWindowFlags_NoResize |
		ImGuiWindowFlags_::ImGuiWindowFlags_NoMove |
		ImGuiWindowFlags_::ImGuiWindowFlags_NoCollapse);

	auto windowPos = ImVec2(static_cast<float>(UI_PADDING), UI_PADDING + MENU_BAR_HEIGHT);
	auto windowSize = ImVec2(static_cast<float>(m_minorWidth - UI_PADDING * 2.0f),
		static_cast<float>(m_resolution.y - m_minorHeight - UI_PADDING - MENU_BAR_HEIGHT));

	ImGui::SetWindowPos("Hierarchy", windowPos);
	ImGui::SetWindowSize("Hierarchy", windowSize);

	//For extra help:
	//https://github.com/ocornut/imgui/issues/324

	//For each child's child we use a recursive lambda to render the
	//name of the object. Each call to 'TreeNode' will add an indented 
	//label of the child and this runs until there are no more children
	std::function<void(Object*)> OpenTree = [&](Object* child)
	{
		//The specific child tab is expanded so display all of its children
		if (ImGui::TreeNode(child->GetTag().c_str()))
		{
			m_activeObject = child;

			for (const auto& child : child->GetChildren())
			{
				OpenTree(child);
			}

			ImGui::TreePop();
		}

		return;
	};

	//The 'Scene' tab is expanded so display all of the grid's children
	if (ImGui::TreeNode(m_grid->GetTag().c_str()))
	{
		for (const auto& child : m_grid->GetChildren())
		{
			OpenTree(child);
		}

		ImGui::TreePop();
	}

	ImGui::End();
}
//======================================================================================================
void Design::RenderPropertiesWindow()
{
	ImGui::Begin("Properties", nullptr,
		ImGuiWindowFlags_::ImGuiWindowFlags_NoTitleBar |
		ImGuiWindowFlags_::ImGuiWindowFlags_NoResize |
		ImGuiWindowFlags_::ImGuiWindowFlags_NoMove |
		ImGuiWindowFlags_::ImGuiWindowFlags_NoCollapse);

	auto windowPos = ImVec2(static_cast<float>(m_majorWidth + UI_PADDING), UI_PADDING + MENU_BAR_HEIGHT);
	auto windowSize = ImVec2(static_cast<float>(m_minorWidth - UI_PADDING * 2.0f),
		static_cast<float>(m_resolution.y - UI_PADDING * 2.0f - MENU_BAR_HEIGHT));

	ImGui::SetWindowPos("Properties", windowPos);
	ImGui::SetWindowSize("Properties", windowSize);

	if (m_activeObject)
	{
		ImGui::TextColored({ 0.0f, 0.56f, 0.8f, 1.0f }, m_activeObject->GetTag().c_str());
		ImGui::Separator();

		for (int i = 0; i < 5; i++)
		{
			ImGui::Spacing();
		}

		ImGui::TextColored({ 0.0f, 0.56f, 0.8f, 1.0f }, "Transform");
		ImGui::Separator();

		static auto isGlobal = false;
		ImGui::Checkbox("Global", &isGlobal);

		static auto isUniformScale = false;
		ImGui::Checkbox("Uniform scale", &isUniformScale);

		ImGui::Spacing();

		auto position = m_activeObject->GetTransform().GetPosition();
		ImGui::SliderFloat3("Position", &position.x, -25.0f, 25.0f, "%.2f");
		m_activeObject->GetTransform().SetPosition(position);

		//TODO - There is a tiny bug here with the sliders
		auto rotation = m_activeObject->GetTransform().GetEulerAngles();
		ImGui::SliderFloat3("Rotation", &rotation.x, -360.0f, 360.0f, "%.2f");
		m_activeObject->GetTransform().SetRotation(rotation);

		auto scale = m_activeObject->GetTransform().GetScale();

		if (isUniformScale)
		{
			ImGui::SliderFloat("Scale", &scale.x, 0.01f, 30.0f, "%.2f");
			m_activeObject->GetTransform().SetScale(glm::vec3(scale.x));
		}

		else
		{
			ImGui::SliderFloat3("Scale", &scale.x, 0.01f, 30.0f, "%.2f");
			m_activeObject->GetTransform().SetScale(scale);
		}

		ImGui::Spacing();

		if (ImGui::Button("Reset", ImVec2(80, 25)))
		{
			m_activeObject->GetTransform().SetIdentity();
		}

		for (int i = 0; i < 5; i++)
		{
			ImGui::Spacing();
		}

		ImGui::TextColored({ 0.0f, 0.56f, 0.8f, 1.0f }, "Material");
		ImGui::Separator();

		auto color = m_activeObject->GetColor();
		ImGui::ColorEdit4("Color", &color.r);
		m_activeObject->SetColor(color);
	}

	else
	{
		ImGui::Text("No object selected");
	}

	ImGui::End();
}