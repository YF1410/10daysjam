#pragma once
class StageManager
{
public:
	enum StageType {
		firstStage,
		secondStage,
		thirdStage,
		fourthStage,
		fifthStage
	};

public:
	void SetStageType(StageType stageType) { nowStageType = stageType; }
	StageType GetStageType() { return nowStageType; }
	StageType GetNextStageType();
private:
	StageType nowStageType;
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

