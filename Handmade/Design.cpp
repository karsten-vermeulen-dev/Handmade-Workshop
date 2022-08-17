#include <functional>

#include "imgui/imgui.h"
#include "imgui/imgui_impl_opengl3.h"
#include "imgui/imgui_impl_sdl.h"
#include <openvr.h>

#include "BoxCollider.h"
#include "Design.h"
#include "Input.h"
#include "Screen.h"
#include "Utility.h"

//======================================================================================================
bool Design::OnEnter()
{
	//TODO - Use a color picker to change this
	Screen::Instance()->SetColor(30U, 30U, 30U);

	mainShader = std::make_unique<Shader>();

	if (!mainShader->Create("Shaders/Main.vert", "Shaders/Main.frag"))
	{
		return false;
	}

	mainShader->BindAttribute("vertexIn");
	mainShader->BindAttribute("colorIn");
	mainShader->BindAttribute("textureIn");

	mainShader->BindUniform("model");
	mainShader->BindUniform("view");
	mainShader->BindUniform("projection");
	mainShader->BindUniform("isTextured");

	consoleLog.push_front("'Main.vert' and 'Main.frag' shaders created and compiled.");

	//===================================================================

	textShader = std::make_unique<Shader>();

	if (!textShader->Create("Shaders/Text.vert", "Shaders/Text.frag"))
	{
		return false;
	}

	textShader->BindAttribute("vertexIn");
	textShader->BindAttribute("colorIn");
	textShader->BindAttribute("textureIn");

	textShader->BindUniform("model");
	textShader->BindUniform("view");
	textShader->BindUniform("projection");
	textShader->BindUniform("textureImage");

	consoleLog.push_front("'Text.vert' and 'Text.frag' shaders created and compiled.");

	//===================================================================
	//TODO - There is a bug with the lighting shaders

	lightShader = std::make_unique<Shader>();

	if (!lightShader->Create("Shaders/Light.vert", "Shaders/Light.frag"))
	{
		return false;
	}

	lightShader->BindAttribute("vertexIn");
	lightShader->BindAttribute("colorIn");
	lightShader->BindAttribute("textureIn");
	lightShader->BindAttribute("normalIn");

	lightShader->BindUniform("model");
	lightShader->BindUniform("view");
	lightShader->BindUniform("projection");
	lightShader->BindUniform("normal");

	lightShader->BindUniform("isTextured");
	lightShader->BindUniform("cameraPosition");

	lightShader->BindUniform("light.ambient");
	lightShader->BindUniform("light.diffuse");
	lightShader->BindUniform("light.specular");
	lightShader->BindUniform("light.position");

	lightShader->BindUniform("material.ambient");
	lightShader->BindUniform("material.diffuse");
	lightShader->BindUniform("material.specular");
	lightShader->BindUniform("material.shininess");

	//lightShader->BindUniform("light.attenuationLinear");
	//lightShader->BindUniform("light.attenuationConstant");
	//lightShader->BindUniform("light.attenuationQuadratic");

	consoleLog.push_front("'Light.vert' and 'Light.frag' shaders created and compiled.");

	//TEST CODE to be used later for multiple lights
	/*for (size_t i = 0; i < TOTAL_LIGHTS; i++)
	{
		std::string index = std::to_string(i);

		 lightShader->BindUniform("lights[" + index + "].ambient");
		 lightShader->BindUniform("lights[" + index + "].diffuse");
		 lightShader->BindUniform("lights[" + index + "].specular");
		 lightShader->BindUniform("lights[" + index + "].position");
		 lightShader->BindUniform("lights[" + index + "].attenuationConstant");
		 lightShader->BindUniform("lights[" + index + "].attenuationLinear");
		 lightShader->BindUniform("lights[" + index + "].attenuationQuadratic");
	}*/

	//===================================================================

	//Find a different way to load standard default materials
	//Material::Load("Defaults", "Defaults.mtl");
	consoleLog.push_front("Default materials loaded.");

	//===================================================================

	//TODO - Move these to a global messaging system so 
	//that the 'App' class can register these messages
	// consoleLog.push_front("Text sub-system initialized.");
	// consoleLog.push_front("Audio sub-system initialized.");

	//=========================================================================

	ImGui::GetIO().Fonts->AddFontFromFileTTF("Assets/Fonts/Quikhand.ttf", fontSize);
	ImGui::GetIO().Fonts->Build();

	//WIP======================================================================
	//Adding objects to the scene
	//========================================================================= 

	grid = std::make_unique<Grid>("Scene");
	grid->GetTransform().SetRotation(45.0f, -30.0f, 0.0f);

	sceneCamera = std::make_unique<FreeCamera>("Main_cam");
	sceneCamera->SetVelocity(0.0f);
	sceneCamera->SetSensitivity(0.0f);
	sceneCamera->GetTransform().SetPosition(0.0f, 0.0f, 50.0f);
	sceneCamera->SetBackgroundColor(29U, 29U, 29U);
	consoleLog.push_front("Scene camera created.");

	light = std::make_unique<Light>("Main_light", 0.0f, 7.5f, 0.0f);

	//=========================================================================

	/* topText = std::make_unique<Text>("Quikhand", "Quikhand.ttf", 30);
	 topText->SetColor(1.0f, 0.0f, 0.196f, 1.0f);
	 topText->SetString("Handmade Alpha");*/

	 /* bottomText = std::make_unique<Text>("Quikhand");
	  bottomText->SetColor(0.0f, 0.564f, 1.0f, 1.0f);
	  bottomText->SetString("Click/Drag the mouse to rotate the grid. Use the mouse wheel to zoom in/out");*/

	  /* axesLabelText = std::make_unique<Text>("Arial", "Arial.ttf", 66);
	   axesLabelText->SetColor(1.0f, 1.0f, 1.0f, 1.0f);
	   axesLabelText->SetString("X");*/

	   /* text.push_back(Text("Quikhand", "Quickhand.ttf", 10));
		text.back().SetColor(1.0f, 0.75f, 0.1f);
		text.back().SetString("Text #1");

		text.push_back(Text("Quikhand"));
		text.back().SetColor(1.0f, 0.75f, 0.1f);
		text.back().SetString("Text #2");

		text.push_back(Text());
		text.back().SetFont("Quickhand");
		text.back().SetColor(1.0f, 0.75f, 0.1f);
		text.back().SetString("Text #3");*/

		/*Audio::Load(Audio::Type::Music, "Test1", "Armin.wav");
		Audio::Load(Audio::Type::Music, "Test2", "Dance.mp3");
		Audio::Load(Audio::Type::Music, "Test3", "Journeys.ogg");

		 audio1 = std::make_unique<Audio>(Audio::Type::Music, "Test1");
		 audio2 = std::make_unique<Audio>(Audio::Type::Music, "Test2");
		 audio3 = std::make_unique<Audio>(Audio::Type::Music, "Test2");

		 audio2->Play();*/

		 //==========================================================================

		 // model = std::make_unique<Model>("Teapot", "Teapot.obj", true);
		 // model->GetTransform().SetScale(5.0f, 5.0f, 5.0f);
		 // model->SetColor(1, 0, 1, 1);

		 // sphere = std::make_unique<Sphere>(10.0f, 50.0f, 50.0f);

	return true;
}
//======================================================================================================
State* Design::Update(int deltaTime)
{
	Audio::Update();

	if (Input::Instance()->IsXClicked() || isStateComplete)
	{
		return nullptr;
	}

	//==============================================================================

	BoxCollider sceneBox;
	auto dimension = sceneCamera->GetResolution();

	sceneBox.SetPosition(minorWidth + (dimension.x * 0.5f), dimension.y * 0.5f, 0.0f);
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
		auto camPos = sceneCamera->GetTransform().GetPosition();
		camPos.z -= (Input::Instance()->GetMouseWheel().y);
		sceneCamera->GetTransform().SetPosition(camPos);

		//Rotate grid
		if (Input::Instance()->IsLeftButtonClicked())
		{
			auto mouseMotion = Input::Instance()->GetMouseMotion();
			sceneRotation.x += -mouseMotion.y;
			sceneRotation.y += mouseMotion.x;
			grid->GetTransform().SetRotation(sceneRotation);
		}
	}

	//==============================================================================

	for (const auto& object : objects)
	{
		if (object->IsActive())
		{
			object->Update(static_cast<GLfloat>(deltaTime));
		}
	}

	//==============================================================================

	resolution = Screen::Instance()->GetResolution();
	minorWidth = static_cast<GLsizei>(resolution.x * minor);
	majorWidth = static_cast<GLsizei>(resolution.x * major);
	minorHeight = static_cast<GLsizei>(resolution.y * minor);
	majorHeight = static_cast<GLsizei>(resolution.y * major);

	return this;
}
//======================================================================================================
bool Design::Render()
{
	auto& mainShader = *(this->mainShader.get());
	auto& textShader = *(this->textShader.get());
	auto& lightShader = *(this->lightShader.get());
	auto& testShader = *(this->testShader.get());

	auto SetViewport = [](const glm::ivec4& viewport, const glm::uvec4& color)
	{
		Screen::Instance()->SetViewport(viewport.x, viewport.y, viewport.z, viewport.w);
		Screen::Instance()->SetColor(color);
		Screen::Instance()->Refresh();
	};

	//Hierarchy viewport
	SetViewport(glm::ivec4(0, minorHeight, minorWidth, resolution.y - minorHeight - menuBarHeight),
		glm::uvec4(0U, 144U, 255U, 1U));

	//Console viewport
	SetViewport(glm::ivec4(0, 0, majorWidth, minorHeight),
		glm::uvec4(0U, 144U, 255U, 1U));

	//Properties viewport
	SetViewport(glm::ivec4(majorWidth, 0, minorWidth, resolution.y),
		glm::uvec4(0U, 144U, 255U, 1U));

	//Scene viewport
	sceneCamera->SetViewport(minorWidth, minorHeight, resolution.x - (minorWidth * 2), majorHeight);
	sceneCamera->CreatePerspView();

	mainShader.Use();

	sceneCamera->Update(16.0f);
	sceneCamera->SendToShader(mainShader);

	//==============================================================================

	grid->Render(mainShader);
	
	lightShader.Use();
	lightShader.SendData("cameraPosition", sceneCamera->GetTransform().GetPosition());
	
	light->SendToShader(lightShader);
	sceneCamera->SendToShader(lightShader);
	
	light->Render(lightShader);

	for (const auto& object : objects)
	{
		if (object->IsLit())
		{
			lightShader.Use();
			object->Render(lightShader);
		}

		else
		{
			mainShader.Use();
			object->Render(mainShader);
		}
	}

	// axes->GetTransform().SetRotation( grid->GetTransform().GetRotation());
	// axes->Render(lightShader);

	// cube->GetTransform().SetRotation( grid->GetTransform().GetRotation());
	// cube->Render(lightShader);

	// model->GetTransform().SetRotation( grid->GetTransform().GetRotation());
	// model->Render(lightShader);

	//==============================================================================
	//Text rendering & UI
	//==============================================================================

	/*textShader.Use();

	 sceneCamera->CreateOrthoView();
	 sceneCamera->Update(16.0f);
	 sceneCamera->SendToShader(textShader);

	auto labelPosition =  sceneCamera->ConvertWorldToScreen( axes->GetArrowTipPositionX());
	 axesLabelText->IsFirstLetterCentered(true);
	 axesLabelText->GetTransform().SetPosition(labelPosition.x, labelPosition.y, 0.0f);
	 axesLabelText->SetString("X");
	 axesLabelText->SendToShader(textShader);
	 axesLabelText->Render(textShader);*/

	 //labelPosition =  mainCamera->ConvertWorldToScreen( axes->GetArrowTipPositionY());
	 // axesLabelText->GetTransform().SetPosition(labelPosition.x, labelPosition.y, 0.0f);
	 // axesLabelText->SendToShader(textShader);
	 // axesLabelText->SetString("Y");
	 // axesLabelText->Render(textShader);

	 //labelPosition =  mainCamera->ConvertWorldToScreen( axes->GetArrowTipPositionZ());
	 // axesLabelText->GetTransform().SetPosition(labelPosition.x, labelPosition.y, 0.0f);
	 // axesLabelText->SendToShader(textShader);
	 // axesLabelText->SetString("Z");
	 // axesLabelText->Render(textShader);

	 //For current testing
	 /*auto count = 0;

	 for (auto& text :  text)
	 {
		 text.GetTransform().SetPosition(10.0f, (resolution.y - 50.0f - count * 100.0f), 0.0f);
		 text.SendToShader(textShader);
		 text.Render(textShader);
		 count++;
	 }*/

	 // quad->GetTransform().SetRotation( grid->GetTransform().GetRotation());
	 // quad->Render(*Shader::Instance());

	 // sphere->GetTransform().SetRotation( grid->GetTransform().GetRotation());
	 // sphere->Render(*Shader::Instance());

	 //TEST CODE to be used later on
	 /* UICamera->SetOrthoView();
	  UICamera->Update();

	 glm::vec2 pixels =  mainCamera->ConvertWorldToScreen( axes->GetArrowTipPositionX());
	  labelX->GetTransform().SetPosition(pixels.x, pixels.y, 0.0f);
	  labelX->Render();

	 pixels =  mainCamera->ConvertWorldToScreen( axes->GetArrowTipPositionY());
	  labelY->GetTransform().SetPosition(pixels.x, pixels.y, 0.0f);
	  labelY->Render();

	 pixels =  mainCamera->ConvertWorldToScreen( axes->GetArrowTipPositionZ());
	  labelZ->GetTransform().SetPosition(pixels.x, pixels.y, 0.0f);
	  labelZ->Render();*/

	  /*for (const auto& object :  objects)
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
	objects.clear();
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
				isStateComplete = true;
			}

			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Objects"))
		{
			if (ImGui::MenuItem("Quad", nullptr, nullptr))
			{
				objects.emplace_back(std::make_unique<Quad>("Quad_" + std::to_string(Quad::GetTotalQuads() + 1)));
				grid->AddChild(objects.back().get());
			}

			if (ImGui::MenuItem("Circle", nullptr, nullptr))
			{
				objects.emplace_back(std::make_unique<Circle>("Circle_" + std::to_string(Circle::GetTotalCircles() + 1)));
				grid->AddChild(objects.back().get());
			}

			if (ImGui::MenuItem("Cuboid", nullptr, nullptr))
			{
				objects.emplace_back(std::make_unique<Cuboid>("Cube_" + std::to_string(Cuboid::GetTotalCuboids() + 1)));
				grid->AddChild(objects.back().get());
			}

			if (ImGui::MenuItem("Sphere", nullptr, nullptr))
			{
				objects.emplace_back(std::make_unique<Sphere>("Sphere_" + std::to_string(Sphere::GetTotalSpheres() + 1)));
				grid->AddChild(objects.back().get());
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
				objects.emplace_back(std::make_unique<Model>("Test_model", "Teapot.obj"));
				grid->AddChild(objects.back().get());
			}

			if (ImGui::MenuItem("Shader...", nullptr, nullptr))
			{
				//Handle menu item...
			}

			if (ImGui::MenuItem("Audio...", nullptr, nullptr))
			{
				//Handle menu item...
			}

			if (ImGui::MenuItem("VR", nullptr, nullptr))
			{
				//TODO - Find a better home for this============================================
				//For reference:															
				//https://github.com/terminal29/Simple-OpenVR-Driver-Tutorial					

				vr::EVRInitError* peError = nullptr;
				vr::VR_Init(peError, vr::EVRApplicationType::VRApplication_Scene);
				//==============================================================================
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

	auto windowPos = ImVec2(paddingUI,
		static_cast<float>(majorHeight + paddingUI + 1.0f));
	auto windowSize = ImVec2(static_cast<float>(majorWidth - paddingUI),
		static_cast<float>(minorHeight - paddingUI * 2.0f));

	ImGui::SetWindowPos("Output console", windowPos);
	ImGui::SetWindowSize("Output console", windowSize);

	if (consoleLog.size() > maxConsoleLogSize)
	{
		consoleLog.pop_back();
	}

	for (auto& log : consoleLog)
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

	auto windowPos = ImVec2(static_cast<float>(paddingUI), paddingUI + menuBarHeight);
	auto windowSize = ImVec2(static_cast<float>(minorWidth - paddingUI * 2.0f),
		static_cast<float>(resolution.y - minorHeight - paddingUI - menuBarHeight));

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
			activeObject = child;

			for (const auto& child : child->GetChildren())
			{
				OpenTree(child);
			}

			ImGui::TreePop();
		}

		return;
	};

	//The 'Scene' tab is expanded so display all of the grid's children
	if (ImGui::TreeNode(grid->GetTag().c_str()))
	{
		for (const auto& child : grid->GetChildren())
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

	auto windowPos = ImVec2(static_cast<float>(majorWidth + paddingUI), paddingUI + menuBarHeight);
	auto windowSize = ImVec2(static_cast<float>(minorWidth - paddingUI * 2.0f),
		static_cast<float>(resolution.y - paddingUI * 2.0f - menuBarHeight));

	ImGui::SetWindowPos("Properties", windowPos);
	ImGui::SetWindowSize("Properties", windowSize);

	if (activeObject)
	{
		ImGui::TextColored({ 0.0f, 0.56f, 0.8f, 1.0f }, activeObject->GetTag().c_str());
		ImGui::Separator();
		
		bool isLit = activeObject->IsLit();
		ImGui::Checkbox("Add lighting", &isLit);
		activeObject->IsLit(isLit);

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

		auto position = activeObject->GetTransform().GetPosition();
		ImGui::SliderFloat3("Position", &position.x, -25.0f, 25.0f, "%.2f");
		activeObject->GetTransform().SetPosition(position);

		//TODO - There is a tiny bug here with the sliders
		auto rotation = activeObject->GetTransform().GetEulerAngles();
		ImGui::SliderFloat3("Rotation", &rotation.x, -360.0f, 360.0f, "%.2f");
		activeObject->GetTransform().SetRotation(rotation);

		auto scale = activeObject->GetTransform().GetScale();

		if (isUniformScale)
		{
			ImGui::SliderFloat("Scale", &scale.x, 0.01f, 30.0f, "%.2f");
			activeObject->GetTransform().SetScale(glm::vec3(scale.x));
		}

		else
		{
			ImGui::SliderFloat3("Scale", &scale.x, 0.01f, 30.0f, "%.2f");
			activeObject->GetTransform().SetScale(scale);
		}

		ImGui::Spacing();

		if (ImGui::Button("Reset", ImVec2(80, 25)))
		{
			activeObject->GetTransform().SetIdentity();
		}

		for (int i = 0; i < 5; i++)
		{
			ImGui::Spacing();
		}

		ImGui::TextColored({ 0.0f, 0.56f, 0.8f, 1.0f }, "Material");
		ImGui::Separator();

		auto color = activeObject->GetColor();
		ImGui::ColorEdit4("Color", &color.r);
		activeObject->SetColor(color);
	}

	else
	{
		ImGui::Text("No object selected");
	}

	ImGui::End();
}