#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {

	ofSetFrameRate(30);
	ofSetWindowTitle("openframeworks");

	ofBackground(255);
	ofSetLineWidth(2);

	this->fbo.allocate(ofGetWidth() + 25, ofGetHeight() + 25);
	this->fbo.readToPixels(this->pixels);
	this->pixels_mat = cv::Mat(this->pixels.getHeight(), this->pixels.getWidth(), CV_8UC4, this->pixels.getData());
}

//--------------------------------------------------------------
void ofApp::update() {

	ofSeedRandom(39);

	this->fbo.begin();
	ofClear(0);
	
	ofTranslate(ofGetWindowSize() * 0.5);

	int span = 40;
	for (int x = span * -8; x < span * 8; x += span) {

		for (int y = span * -8; y < span * 8; y += span) {

			vector<glm::vec2> vertices;

			int r = ofMap(ofNoise(ofRandom(1000), ofGetFrameNum() * 0.0035), 0, 1, 0, 4);
			auto location = glm::vec2(x, y);

			if (r == 1) {

				location += glm::vec2(span, 0);
			}
			else if (r == 2) {

				location += glm::vec2(span, span);
			}
			else if (r == 3) {

				location += glm::vec2(0, span);
			}

			int deg_start = r * 90;
			int radius = span;
			for (int deg = deg_start; deg <= deg_start + 90; deg += 45) {

				vertices.push_back(location + glm::vec2(radius * cos(deg * DEG_TO_RAD), radius * sin(deg * DEG_TO_RAD)));
			}

			r = (r + 2) % 4;

			location = glm::vec2(x, y);
			if (r == 1) {

				location += glm::vec2(span, 0);
			}
			else if (r == 2) {

				location += glm::vec2(span, span);
			}
			else if (r == 3) {

				location += glm::vec2(0, span);
			}

			deg_start = r * 90;
			for (int deg = deg_start; deg <= deg_start + 90; deg += 45) {

				vertices.push_back(location + glm::vec2(radius * cos(deg * DEG_TO_RAD), radius * sin(deg * DEG_TO_RAD)));
			}

			ofSetColor(0);
			ofNoFill();
			ofBeginShape();
			ofVertices(vertices);
			ofEndShape(true);
		}
	}

	this->fbo.end();
	this->fbo.readToPixels(this->pixels);

	cv::Mat gray_mat, bit_mat;
	cv::cvtColor(this->pixels_mat, gray_mat, cv::COLOR_RGB2GRAY);
	for (int y = 0; y < this->pixels_mat.cols; y++) {

		for (int x = 0; x < this->pixels_mat.rows; x++) {

			if (gray_mat.at<unsigned char>(y, x) > 0) {

				this->pixels_mat.at<cv::Vec4b>(y, x) = cv::Vec4b(139, 139, 139, 255);
			}
		}
	}

	cv::GaussianBlur(this->pixels_mat, this->pixels_mat, cv::Size(19, 19), 10, 10);
}

//--------------------------------------------------------------
void ofApp::draw() {

	ofSetColor(255);

	ofImage draw_image;
	draw_image.setFromPixels(this->pixels);
	draw_image.draw(0, 0);

	this->fbo.draw(-10, -10);
}

//--------------------------------------------------------------
int main() {

	ofSetupOpenGL(720, 720, OF_WINDOW);
	ofRunApp(new ofApp());
}