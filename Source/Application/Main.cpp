#include "World01.h"
#include "World02.h"
#include "World03.h"
#include "World04.h"
#include "World05.h"
#include "World06.h"
#include "Core/Core.h"
#include "Framework/Framework.h"
#include "Renderer/Renderer.h"

using namespace std;

int main(int argc, char* argv[]) {
	INFO_LOG("Initialize Engine...")

	//nc::MemoryTracker::Initialize();
	nc::seedRandom((unsigned int)time(nullptr));
	nc::setFilePath("assets");

	ENGINE.Initialize();

	string dog = "Dog";
	string DOG = "dOG";
	string dOg = "dOgG";
	cout << nc::ToUpper(dog) << endl;
	cout << nc::ToLower(DOG) << endl;
	cout << nc::IsEqualIgnoreCase(dog, DOG) << endl;
	cout << nc::IsEqualIgnoreCase(dog, dOg) << endl;
	nc::CreateUnique(dog);
	cout << dog << endl;
	cout << dog << endl;
	nc::CreateUnique(dog);
	cout << dog << endl;
	cout << dog << endl;
	
	//auto world = make_unique<nc::World01>();
	//auto world = make_unique<nc::World02>();
	//auto world = make_unique<nc::World03>();
	//auto world = make_unique<nc::World04>();
	//auto world = make_unique<nc::World05>();
	auto world = make_unique<nc::World06>();
	world->Initialize();

	// main loop
	bool quit = false;
	while (!quit) {
		// update
		ENGINE.Update();
		quit = ENGINE.IsQuit();

		world->Update(ENGINE.GetTime().GetDeltaTime());

		// draw
		world->Draw(*ENGINE.GetSystem<nc::Renderer>());
	}

	world->Shutdown();
	ENGINE.Shutdown();

	return 0;
}
