#include "World01.h"
#include "Framework/Framework.h"
#include "Renderer/Renderer.h"
#include "Input/InputSystem.h"

namespace nc {
    bool World01::Initialize() {
        return true;
    }

    void World01::Shutdown() {
    }

    void World01::Update(float dt) {
    }

    void World01::Draw(Renderer& renderer) {
        // pre-render
        renderer.BeginFrame();

        // render
        glBegin(GL_LINE);

        // post-render
        renderer.EndFrame();
    }
}
