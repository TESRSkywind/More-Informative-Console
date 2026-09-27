#include "TESModel.h"
#include "Util/FilePathUtil.h"
#include "TranslationCache.h"

//4-30-2022: Checked for translations needed

void AddModelEntry(ExtraInfoEntry* resultArray, std::string modelType, RE::TESModel* model, priority priority)
{
	REX::DEBUG("Starting AddModelEntry for model");

	if (model) {
		std::string modelPath = model->GetModel();
		ExtraInfoEntry* modelPathEntry;
		CreateExtraInfoEntry(modelPathEntry, modelType, modelPath, priority);
		resultArray->PushBack(modelPathEntry);
		
		REX::DEBUG("Ending AddModelEntry for model");
		/*
		if (modelPath != "") {
			REX::DEBUG("Get Model path");

			std::string modelName = GetFileName(modelPath);

			REX::DEBUG("Get Model name");

			ExtraInfoEntry* modelEntry;
			CreateExtraInfoEntry(modelEntry, modelType, modelName, priority);

			//Create an entry for the model path
			REX::DEBUG("Splitting Model Path");
			
			ExtraInfoEntry* modelPathEntry;
			CreateExtraInfoEntry(modelPathEntry, GetTranslation("$ModelPath"), "", priority_Model);


			CreateFilePathSubarray(modelPathEntry, modelPath);
			modelEntry->PushBack(modelPathEntry);

			REX::DEBUG("Done Splitting Model Path");
			
			resultArray->PushBack(modelEntry);

			RE::TESDataHandler* handler = RE::TESDataHandler::GetSingleton();

			REX::DEBUG(IntToString( handler->compiledFileCollection.smallFiles.size()).c_str() ) ;
			//REX::DEBUG(IntToString(handler->files.end).c_str());
		}*/
	}

}
/*
void AddTextureEntry(ExtraInfoEntry* resultArray, RE::TESModel* model, int textureNumber)
{
	RE::BSResource::ID* texture = model->textures[textureNumber];
}*/
