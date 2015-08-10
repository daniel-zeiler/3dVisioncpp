#include "Init_GLUT.h"

using namespace Core::Init;

Core::IListener* Init_GLUT::listener = NULL;
Core::WindowInfo Init_GLUT::windowInformation;

void Init_GLUT::Init(const Core::WindowInfo&  windowInfo, const Core::ContextInfo& contextInfo, const Core::FramebufferInfo& framebufferInfo){
	int fakeargc = 1;
	char *fakeargv[] = { "fake", NULL };
	glutInit(&fakeargc, fakeargv);
	windowInformation = windowInfo;
	if (contextInfo.core){
		glutInitContextVersion(contextInfo.major_version, contextInfo.minor_version);
		glutInitContextProfile(GLUT_CORE_PROFILE);
	}
	else{
		glutInitContextProfile(GLUT_COMPATIBILITY_PROFILE);
	}

	glutInitDisplayMode(framebufferInfo.flags);
	glutInitWindowPosition(windowInfo.position_x, windowInfo.position_y);
	glutInitWindowSize(windowInfo.width, windowInfo.height);

	glutCreateWindow(windowInfo.name.c_str());
	std::cout << "GLUT:initialized" << std::endl;
	glEnable(GL_DEBUG_OUTPUT);
	glutIdleFunc(IdleCallback);
	glutCloseFunc(CloseCallback);
	glutDisplayFunc(DisplayCallback);
	glutReshapeFunc(ReshapeCallback);

	Core::Init::Init_GLEW::Init();

	//cleanup
	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);

}

void Init_GLUT::Run(){
	std::cout << "GLUT:\t Start Running  " << std::endl;
	glutMainLoop();
}

void Init_GLUT::Close(){
	std::cout << "GLUT:\t Finished" << std::endl;
	glutLeaveMainLoop();
}

void Init_GLUT::IdleCallback(void){
	glutPostRedisplay();
}

void Init_GLUT::DisplayCallback(){
	if (listener){
		listener->NotifyBeginFrame();
		listener->NotifyDisplayFrame();

		glutSwapBuffers();

		listener->NotifyEndFrame();
	}
}

void Init_GLUT::ReshapeCallback(int width, int height){
	if (windowInformation.isReshapable) {
		if (listener)
			listener->NotifyReshape(width, height, windowInformation.width, windowInformation.height);

		windowInformation.width = width;
		windowInformation.height = height;
	}
}

void Init_GLUT::CloseCallback(){
	Close();
}

void Init_GLUT::EnterFullscreen(){
	glutFullScreen();
}

void Init_GLUT::ExitFullscreen(){
	glutLeaveFullScreen();
}

void Init_GLUT::SetListener(Managers::Scene_Manager*& iListener){
	listener = iListener;
}