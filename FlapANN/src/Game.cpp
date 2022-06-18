#include "pch.h"
#include "Game.h"

#include <imgui-sfml/imgui-SFML.h>
#include <imgui/imgui.h>

#include "nodes/objects/bird/Bird.h"
#include "nodes/objects/pipe/Pipe.h"
#include "nodes/objects/background/Background.h"
#include "nodes/objects/background/Ground.h"

const sf::Time Game::TIME_PER_FRAME = sf::seconds(1.f / 60.f);
const int Game::GAME_WIDTH = 144;
const int Game::GAME_HEIGHT = 256;
const int Game::SCREEN_SCALE = 3;
const int Game::IMGUI_SIDEMENU_WIDTH = GAME_WIDTH;
float Game::TIME_SPEED_SCALAR = 1.f;


Game::Game():
	mGameWindow(sf::VideoMode(GAME_WIDTH + IMGUI_SIDEMENU_WIDTH, GAME_HEIGHT), "FlapANN", sf::Style::Titlebar | sf::Style::Close)
{
	// Limit the framerate to 60 frames per second
	mGameWindow.setFramerateLimit(60);
	mGameWindow.setKeyRepeatEnabled(false);

	// Makes window bigger
	mGameWindow.setSize({(GAME_WIDTH + IMGUI_SIDEMENU_WIDTH) * SCREEN_SCALE, GAME_HEIGHT * SCREEN_SCALE });

	// load resources
	loadResources();

	mGameManager = std::make_unique<GameManager>(mTextures, sf::Vector2u{GAME_WIDTH, GAME_HEIGHT}, mFonts);
	ImGui::SFML::Init(mGameWindow);
}

void Game::run()
{
	// It controls the flow of the game loop
	// So the game is not framerate-dependent
	// so it works the same no matter what
	// performance has the player

	sf::Clock clock;
	auto frameTimeElapsed = sf::Time::Zero;
	while (mGameWindow.isOpen())
	{
		if (TIME_SPEED_SCALAR >= 1.f)
		{
			frameTimeElapsed += clock.restart() * TIME_SPEED_SCALAR;
		}
		else
		{
			frameTimeElapsed += clock.restart();
		}
		while (frameTimeElapsed > TIME_PER_FRAME)
		{
			// Update world no more than 60 frames per seconds
			frameTimeElapsed -= TIME_PER_FRAME;
			processEvents();
			if (TIME_SPEED_SCALAR >= 1.f)
			{
				update(TIME_PER_FRAME);
			}
			else
			{
			    update(TIME_PER_FRAME * TIME_SPEED_SCALAR);
			}
		}
		ImGui::SFML::Update(mGameWindow, frameTimeElapsed);
		updateImGui();
		render();
	}
	mGameWindow.close();
	ImGui::SFML::Shutdown();
}

void Game::processEvents()
{
	sf::Event event;
	while (mGameWindow.pollEvent(event))
	{
		if (event.type == sf::Event::Closed)
			mGameWindow.close();

		ImGui::SFML::ProcessEvent(event);

		// Ignore all events that are related directly with ImGui
		if (ImGui::IsWindowHovered(ImGuiFocusedFlags_AnyWindow) || ImGui::IsAnyItemActive())
			return;

		// process event there
		mGameManager->handleEvents(event);
	}
}

void Game::update(const sf::Time& deltaTime)
{
	mGameManager->update(deltaTime);
}

void Game::updateImGui()
{
	ImGui::SetNextWindowPos(ImVec2(GAME_WIDTH * SCREEN_SCALE, 0), ImGuiCond_Once);
	ImGui::SetNextWindowSize(ImVec2(IMGUI_SIDEMENU_WIDTH * SCREEN_SCALE, GAME_HEIGHT * SCREEN_SCALE), ImGuiCond_Once);
	ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.1f, 0.1f, 0.1f, 1.0f)); 
	ImGui::Begin("DefaultSettings", nullptr, 
		ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar | 
		ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);
	ImGui::Text("Game settings:");
    const static auto& sliderText = "Speed of the game";
	const static auto& textSize = ImGui::CalcTextSize(sliderText);

	ImGui::PushItemWidth(-textSize.x);
	ImGui::SliderFloat(sliderText, &TIME_SPEED_SCALAR, 0.f, 5.f);
    mGameManager->updateImGui();
	ImGui::End();
	ImGui::PopStyleColor();
}

void Game::render()
{
	// before drawing anything clean
	// the previous frame
	mGameWindow.clear();

	mGameWindow.draw(*mGameManager);

	mGameWindow.pushGLStates();
	ImGui::SFML::Render(mGameWindow);
	mGameWindow.popGLStates();

	// display to the window
	mGameWindow.display();
}


void Game::loadResources()
{
	mFonts.storeResource(Fonts_ID::ArialNarrow, "resources/fonts/arial_narrow.ttf");

	Bird::loadResources(mTextures);
	Pipe::loadResources(mTextures);
	Background::loadResources(mTextures);
	Ground::loadResources(mTextures);
}
