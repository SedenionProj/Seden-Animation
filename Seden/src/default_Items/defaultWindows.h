#pragma once
#include <string>
#include <ImSequencer.h>
#include <ImGuizmo.h>

#include "src/plugin_api/GUIwindow.h"
#include "src/applicationInfo.h"
#include "src/ressource_manager/ShaderSystem.h"
#include "src/logger.h"
#include "src/default_Items/defaultEffects.h"
#include "src/default_Items/defaultShapes.h"
#include "src/default_Items/defaultTheme.h"

namespace Seden {

	class CompositionWin : public GuiWindow {
	public:
		CompositionWin(AppInfo& appInfo)
			: m_appInfo(appInfo) {}
		virtual void draw() override {
			auto scene = m_appInfo.getCurrentScene();
			ImGui::Begin("composition");
			ImGui::BeginChild("Render");

			ImVec2 winSize = ImGui::GetWindowSize();
			glm::vec2 vpSize = { scene->viewportInfo.width, scene->viewportInfo.height };
			ImVec2 vpOnWinSize = { winSize.y * vpSize.x / vpSize.y, winSize.y };

			ImGui::SetCursorPos({ (winSize.x - vpOnWinSize.x) / 2.f,0 });
			ImGui::Image((ImTextureID)scene->textureColorbuffer, vpOnWinSize);
			ImGui::EndChild();
			ImGui::End();
		}

	private:
		AppInfo& m_appInfo;
	};

	class PrimitivesWin : public GuiWindow {
	public:
		PrimitivesWin(AppInfo& appInfo)
			: m_appInfo(appInfo) {}

		virtual void draw() override {
			ImGui::Begin("Primitives");
			if (ImGui::Button("add square")) {
				auto scene = m_appInfo.getCurrentScene();
				scene->addShape(std::make_shared<Quad>(scene));
			};
			if (ImGui::Button("add text")) {
				auto scene = m_appInfo.getCurrentScene();
				scene->addShape(std::make_shared<Text>(scene));
			};
			ImGui::End();
		}
	private:
		AppInfo& m_appInfo;
	};

	class EffectsWin : public GuiWindow {
	public:
		EffectsWin(AppInfo& appInfo)
			: m_appInfo(appInfo) {
			effectsName.push_back("imported effect");
		}

		virtual void draw() override {
			auto shape = m_appInfo.getCurrentShape();
			ImGui::Begin("Effects");
			if (shape) {
				ImGui::SeparatorText(shape->getName().c_str());
				ImGui::DragInt("instance number", &shape->instanceCount, 0.5, 0, 1000000);
				if (ImGui::BeginMenu("add effect")) {
					for (auto& name : effectsName) {
						if (ImGui::MenuItem(name.c_str())) {
							DEBUG_MSG("added effect");
						};
					}
					ImGui::EndMenu();
				}
				if (ImGui::CollapsingHeader("Properties"))
					shape->drawGui();
				for (auto effect : shape->getEffects()) {
					if (ImGui::CollapsingHeader(effect->getName().c_str())) {
						effect->drawGUI();
					}
				}
			}
			ImGui::End();
		}

	private:
		std::vector<std::string> effectsName;
		AppInfo& m_appInfo;
	};

	struct MySequence : public ImSequencer::SequenceInterface
	{
		MySequence(AppInfo& appInfo) : mFrameMin(0), mFrameMax(100), m_appInfo(appInfo) {}

		virtual int GetFrameMin()  const override { return mFrameMin; }
		virtual int GetFrameMax()  const override { return mFrameMax; }
		virtual void Get(int index, int** start, int** end, int* type, unsigned int* color) {
			auto item = m_appInfo.getShapeByIndex(index);

			if (color)
				*color = 0xFFAA8080;
			if (start)
				*start = &item->mFrameStart;
			if (end)
				*end = &item->mFrameEnd;
			if (type)
				*type = item->mType;
		};

		virtual int GetItemCount() const override { return (int)m_appInfo.getCurrentScene()->getShapes().size(); }
		virtual int GetItemTypeCount() const override { return 0; }
		virtual const char* GetItemTypeName(int typeIndex) const override { return typeIndex == 0 ? "Type1" : "Type2"; }
		virtual const char* GetItemLabel(int index) const override { return m_appInfo.getShapeByIndex(index)->getName().c_str(); }
		virtual void Duplicate(int index) { swapItemPosition(index, index - 1); }
		virtual void Del(int index) { swapItemPosition(index, index + 1); }
		int mFrameMin, mFrameMax;

	private:

		void swapItemPosition(int index1, int index2) {
			auto& sequenceItems = m_appInfo.getCurrentScene()->getShapes();

			if (index1 < 0 || index1 >= sequenceItems.size() || index2 < 0 || index2 >= sequenceItems.size()) {
				return;
			}

			std::swap(sequenceItems[index1], sequenceItems[index2]);
		}

		AppInfo& m_appInfo;
	};

	class Timeline : public GuiWindow {
	public:
		Timeline(AppInfo& appInfo)
			: m_appInfo(appInfo) {

		}

		virtual void draw() {
			auto shape = m_appInfo.getCurrentScene();
			mySequence.mFrameMin = shape->viewportInfo.mFrameMin;
			mySequence.mFrameMax = shape->viewportInfo.mFrameMax;

			ImGui::Begin("Timeline");
			bool res = ImSequencer::Sequencer(&mySequence, &m_appInfo.currentFrame, &expanded, &selectedEntry, &firstFrame,
				ImSequencer::SEQUENCER_EDIT_STARTEND | ImSequencer::SEQUENCER_DEL | ImSequencer::SEQUENCER_COPYPASTE | ImSequencer::SEQUENCER_CHANGE_FRAME);

			if (!ImGui::IsWindowHovered() && ImGui::IsMouseClicked(ImGuiMouseButton_Left))
				selectedEntry = -1;
			ImGui::End();

			if (selectedEntry >= 0) {
				m_appInfo.setCurrentShape(m_appInfo.getShapeByIndex(selectedEntry));
			}
		}
	private:
		int selectedEntry = -1;
		int firstFrame = 0;
		bool expanded = true;

		AppInfo& m_appInfo;
		MySequence mySequence{ m_appInfo };
	};

	class Settings : public GuiWindow {
	public:
		Settings(AppInfo& appInfo)
			: m_appInfo(appInfo) {
		}

		void draw() override {
			ImGui::Begin("Settings", &show_window);

			if (ImGui::BeginMenu("select theme")) {
				if (ImGui::MenuItem("defaultTheme")) {
					ImGui::StyleColorsDark();
				};
				if (ImGui::MenuItem("Cherry")) {
					std::unique_ptr<Cherry> tmp = std::make_unique<Cherry>();
					tmp->select();
				};

				for (auto& name : m_themeNames) {
					if (ImGui::MenuItem(name.c_str())) {
						DEBUG_MSG("theme");
					};
				}
				ImGui::EndMenu();
			}
			ImGui::End();
		}
	private:
		std::vector<std::string> m_themeNames;
		AppInfo& m_appInfo;
	};

	class CompositionSetting : public GuiWindow {
	public:
		CompositionSetting(AppInfo& appInfo)
			: m_appInfo(appInfo) {
		}

		void draw() override {
			auto scene = m_appInfo.getCurrentScene();
			ImGui::Begin("Composition Setting", &show_window);
			if (ImGui::BeginMenu("Projection mode")) {
				if (ImGui::MenuItem("no projection")) { scene->viewportInfo.setProjection(ViewportInfo::NO_PROJECTION); }
				if (ImGui::MenuItem("perspective")) { scene->viewportInfo.setProjection(ViewportInfo::PERSPECTIVE); }
				if (ImGui::MenuItem("orthographic")) { scene->viewportInfo.setProjection(ViewportInfo::ORTHOGRAPHIC); }
				ImGui::EndMenu();
			}
			ImGui::End();
		}

	private:
		AppInfo& m_appInfo;
	};

	class Render : public GuiWindow {
	public:
		Render(AppInfo& appInfo)
			: m_appInfo(appInfo) {}
		void draw() override {
			saveRecordingFrame();

			ImGui::Begin("Render", &show_window);

			ImGui::ProgressBar(progress, ImVec2(ImGui::GetContentRegionAvail().x, 0), nullptr);

			ImGui::Spacing();

			float totalWidth = ImGui::GetContentRegionAvail().x;
			float leftSectionWidth = totalWidth * 0.5f;
			float rightSectionWidth = totalWidth * 0.5f;

			ImGui::Columns(2, nullptr, false);

			// Left Section
			ImGui::SetColumnWidth(0, leftSectionWidth);
			//ImGui::Text("Left Section:");
			//ImGui::BulletText("Item 1");
			//ImGui::BulletText("Item 2");
			//ImGui::BulletText("Item 3");

			ImGui::NextColumn();

			// Right Section
			ImGui::SetColumnWidth(1, rightSectionWidth);
			if (ImGui::Button("start")) { startRecording(); }
			if (ImGui::Button("stop")) { stopRecording(); }

			ImGui::Columns(1);
			ImGui::End();
		}
	private:
		void startRecording() {
			const std::string& path = "C:\\Users\\Gerome\\Desktop\\rawteset";
			auto& viewport = m_appInfo.getCurrentScene()->viewportInfo;
			m_appInfo.currentFrame = viewport.mFrameMin;

			if (!m_isRecording) {
				image = std::make_unique<int[]>(viewport.width * viewport.height);
				std::string cmd = "ffmpeg -r " + std::to_string(viewport.fps) +
					" -f rawvideo -pix_fmt rgba -s " + std::to_string(viewport.width) + "x" + std::to_string(viewport.height) + " -i - " +
					"-threads 0 -preset fast -y -pix_fmt yuv420p -crf 21 -vf vflip " + path + ".mp4";

				ffmpeg = _popen(cmd.c_str(), "wb");
				m_isRecording = true;
				m_appInfo.isContinuousRefresh = true;
			}
		}

		void stopRecording() {
			if (m_isRecording) {
				auto& viewport = m_appInfo.getCurrentScene()->viewportInfo;
				m_appInfo.currentFrame = viewport.mFrameMin;

				m_isRecording = false;
				_pclose(ffmpeg);

				m_appInfo.isContinuousRefresh = false;
			}
		}

		void saveRecordingFrame() {
			if (m_isRecording) {
				auto& viewport = m_appInfo.getCurrentScene()->viewportInfo;

				glBindFramebuffer(GL_FRAMEBUFFER, m_appInfo.getCurrentScene()->framebuffer);
				glReadPixels(0, 0, viewport.width, viewport.height, GL_RGBA, GL_UNSIGNED_BYTE, image.get());
				glBindFramebuffer(GL_FRAMEBUFFER, 0);

				fwrite(image.get(), sizeof(int) * viewport.width * viewport.height, 1, ffmpeg);

				progress = (float)(m_appInfo.currentFrame - viewport.mFrameMin) / (viewport.mFrameMax - viewport.mFrameMin);


				if (m_appInfo.currentFrame >= viewport.mFrameMax)
					stopRecording();

				m_appInfo.currentFrame++;
			}
		}

		bool m_isRecording = false;

		FILE* ffmpeg = nullptr;
		std::unique_ptr<int[]> image;

		float progress = 0.f;
		AppInfo& m_appInfo;
	};

	class TopBar : public GuiWindow {
	public:
		TopBar(AppInfo& appInfo)
			: m_appInfo(appInfo) {}

		void draw() override {
			if (ImGui::BeginMainMenuBar())
			{
				if (ImGui::BeginMenu("File"))
				{
					if (ImGui::MenuItem("Open", "Ctrl+O")) { /* Handle Open action */ }
					if (ImGui::MenuItem("Save", "Ctrl+S")) { /* Handle Save action */ }
					ImGui::Separator();
					if (ImGui::MenuItem("Exit", "Alt+F4")) { /* Handle Exit action */ }
					ImGui::EndMenu();
				}

				if (ImGui::BeginMenu("Edit"))
				{
					if (ImGui::MenuItem("Undo", "Ctrl+Z")) { /* Handle Undo action */ }
					if (ImGui::MenuItem("Redo", "Ctrl+Y", false, false)) { /* Handle Redo action (disabled) */ }
					ImGui::Separator();
					if (ImGui::MenuItem("Cut", "Ctrl+X")) { /* Handle Cut action */ }
					if (ImGui::MenuItem("Copy", "Ctrl+C")) { /* Handle Copy action */ }
					if (ImGui::MenuItem("Paste", "Ctrl+V")) { /* Handle Paste action */ }
					ImGui::EndMenu();
				}

				if (ImGui::BeginMenu("Help"))
				{
					if (ImGui::MenuItem("About")) { /* Handle About action */ }
					ImGui::EndMenu();
				}

				if (ImGui::BeginMenu("Window"))
				{
					if (ImGui::MenuItem("Settings")) {
						m_appInfo.addWindows(new Settings(m_appInfo));
					}
					if (ImGui::MenuItem("Composition Setting")) {
						m_appInfo.addWindows(new CompositionSetting(m_appInfo));
					}
					if (ImGui::MenuItem("render export")) {
						m_appInfo.addWindows(new Render(m_appInfo));
					}
					ImGui::EndMenu();
				}

				ImGui::EndMainMenuBar();
			}
		}

	private:
		AppInfo& m_appInfo;
	};
}