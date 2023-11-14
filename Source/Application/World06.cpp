#include "World06.h"
#include "Framework/Framework.h"
#include "Input/InputSystem.h"
#include <glm/glm/gtc/type_ptr.hpp>
#include <glm/glm/gtx/color_space.hpp>


namespace nc {
    bool World06::Initialize() {
        m_scene = std::make_unique<Scene>();
        m_scene->Load("scenes/scene_framebuffer.json");
        m_scene->Initialize();

        auto texture = std::make_shared<Texture>();
        texture->CreateTexture(2048, 2048);
        ADD_RESOURCE("fb_texture", texture);

        auto framebuffer = std::make_shared<Framebuffer>();
        framebuffer->CreateFramebuffer(texture);
        ADD_RESOURCE("fb", framebuffer);

        auto material = GET_RESOURCE(Material, "materials/postprocess.mtrl");
        if (material) {
            material->albedoTexture = texture;
        }

        for (int i = 0; i < 2; i++) {
            auto actor = CREATE_CLASS_BASE(Actor, "tree");
            actor->name = CreateUnique("tree");
            actor->transform.position = glm::vec3{ randomf(-10, 10), 0, randomf(-10, 10) };
            actor->transform.scale = glm::vec3{ randomf(0.5f, 3.0f), randomf(0.5f, 3.0f), 0 };
            actor->Initialize();
            m_scene->Add(std::move(actor));
        }
        return true;
    }

    void World06::Shutdown() {
    }

    void World06::Update(float dt) {
        m_time += dt;
        ENGINE.GetSystem<Gui>()->BeginFrame();
        m_scene->Update(dt);
        m_scene->ProcessGui();

        // set postprocess gui
        ImGui::Begin("Post-Process");
        ImGui::SliderFloat("Blend", &m_blend, 0, 1);
        bool effect = m_params & INVERT_MASK;
        if (ImGui::Checkbox("Invert", &effect)) {
            // sets params to XXX1 if checkbox is true, sets to XXX0 if false
            if (effect) m_params |= INVERT_MASK;
            else m_params ^= INVERT_MASK;
            //(effect) ? m_params |= INVERT_MASK : m_params &= -INVERT_MASK;
        }
        effect = m_params & GRAYSCALE_MASK;
        if (ImGui::Checkbox("Grayscale", &effect)) {
            // sets params to XX1X if checkbox is true, sets to XX0X if false
            if (effect) m_params |= GRAYSCALE_MASK;
            else m_params ^= GRAYSCALE_MASK;
            //(effect) ? m_params |= GRAYSCALE_MASK : m_params &= -GRAYSCALE_MASK;
        }
        effect = m_params & COLORTINT_MASK;
        if (ImGui::Checkbox("Colortint", &effect)) {
            // sets params to X1XX if checkbox is true, sets to X0XX if false
            if (effect) m_params |= COLORTINT_MASK;
            else m_params ^= COLORTINT_MASK;
            //(effect) ? m_params |= COLORTINT_MASK : m_params &= -COLORTINT_MASK;
        }
        effect = m_params & GRAIN_MASK;
        if (ImGui::Checkbox("Grain", &effect)) {
            if (effect) m_params |= GRAIN_MASK;
            else m_params ^= GRAIN_MASK;
        }
        effect = m_params & SCANLINE_MASK;
        if (ImGui::Checkbox("Scanline", &effect)) {
            if (effect) m_params |= SCANLINE_MASK;
            else m_params ^= SCANLINE_MASK;
        }
        effect = m_params & CUSTOM_MASK;
        if (ImGui::Checkbox("Lens Flare", &effect)) {
            if (effect) m_params |= CUSTOM_MASK;
            else m_params ^= CUSTOM_MASK;
        }
        ImGui::ColorEdit3("Color", glm::value_ptr(m_colorTint));
        ImGui::DragFloat2("Flare Position", glm::value_ptr(m_flarePos));
        ImGui::End();

        // set postprocess shader
        auto program = GET_RESOURCE(Program, "shaders/postprocess.prog");
        if (program) {
            program->Use();
            program->SetUniform("blend", m_blend);
            program->SetUniform("params", m_params);
            program->SetUniform("colorTint", m_colorTint);
            program->SetUniform("flarePos", m_flarePos);
            program->SetUniform("time", m_time);
        }
        
        auto actor = m_scene->GetActorByName<Actor>("actor1");

        auto material = actor->GetComponent<ModelComponent>()->m_material;
        material->ProcessGui();
        material->Bind();

        material = GET_RESOURCE(Material, "Materials/refraction.mtrl");
        if (material) {
            ImGui::Begin("Refraction");
            ImGui::SliderFloat("IOR", &m_refraction, 1, 3);
            auto program = material->GetProgram();
            program->Use();
            program->SetUniform("ior", m_refraction);
            ImGui::End();
        }
    }

    void World06::Draw(Renderer& renderer) {
        // *** PASS 1 ***
        m_scene->GetActorByName("postprocess")->active = false;

        auto framebuffer = GET_RESOURCE(Framebuffer, "fb");
        renderer.SetViewport(framebuffer->GetSize().x, framebuffer->GetSize().y);
        framebuffer->Bind();

        renderer.BeginFrame();
        m_scene->Draw(renderer);

        framebuffer->Unbind();

        // *** PASS 2 ***
        m_scene->GetActorByName("postprocess")->active = true;

        renderer.ResetViewport();
        renderer.BeginFrame();
        //m_scene->Draw(renderer);
        m_scene->GetActorByName("postprocess")->Draw(renderer);

        ENGINE.GetSystem<Gui>()->Draw();

        renderer.EndFrame();
    }
}
