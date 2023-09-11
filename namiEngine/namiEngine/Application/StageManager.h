#pragma once
class StageManager
{
public:
	void SetScore(int score) { this->score = score; }
	int GetScore() { return score; }
private:
	int score;
public:
	static StageManager* GetInstance();
private:
	static StageManager instance;
private:
	StageManager() = default;
	StageManager(const StageManager&) = delete;
	~StageManager() = default;
	StageManager& operator=(const StageManager&) = delete;
};

