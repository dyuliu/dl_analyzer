#include <libanalyzer/include/recorder/recorder.h>
#define MAP_TYPE_RECORD std::map<std::string, std::string>

namespace db {

	using TYPE_RECORD = analyzer::Recorders::TYPE_RECORD;

	MAP_TYPE_RECORD mapTypeRecord = {
		{ "train_error", "RecTrainError" },
		{ "train_loss", "RecTrainLoss" },
		{ "test_error", "RecTestError" },
		{ "test_loss", "RecTestLoss" },
		{ "forward_time", "RecForwardTime" },
		{ "backward_time", "RecBackwardTime" },
		{ "update_time", "RecUpdateTime" },
		{ "learning_rate", "RecLearningRate" }
	};

}