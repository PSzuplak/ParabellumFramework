#include "stdafx.h"
#include "ShaderConverter.h"

//	--------------------------------------------------------------------
//		Constructors
//	--------------------------------------------------------------------

ParabellumFramework::ShaderConverter::ShaderConverter()
{
}

ParabellumFramework::ShaderConverter::ShaderConverter(const ShaderConverter& ref)
{

}

ParabellumFramework::ShaderConverter::~ShaderConverter()
{

}



ParabellumFramework::EUINT32 
ParabellumFramework::ShaderConverter::Convert(_IN_ char* filenameIn, _IN_ char* filenameOut)
{
	// in
	PFDLOG("PFGI", "CLSC", "ShaderConverter - converting file - in :  " << filenameIn << ", out " << filenameOut);

	struct HLSL_Pass
	{
		std::string vsFunctionName;		// vertx shader function name
		std::string psFunctionName;		// pixel shader function name
		std::string gsFunctionName;		// geometry shader function name
		std::string tsFunctionName;		// tesel shader function name

		std::string vsVersion;
		std::string psVersion;
		std::string gsVersion;
		std::string tsVersion;
	};

	struct HLSL_VSLayoutVariable
	{
		std::string type;
		std::string name;
		EUINT32 semantic;
		std::string semanticFName;
		EUINT32 semanticIndex;
		EUINT32 format;
	};



	struct HLSL_Variable
	{	
		std::string name;
		EUINT32 type;
		EUINT32 key;
		EUINT32 size;
	};

	struct HLSL_CBuffer
	{
		EUINT32 number;
		std::string name;
		std::vector<HLSL_Variable> variables;
	};


	// load whole file to string
	std::fstream fio;
	fio.open(filenameIn);

	if (fio.fail())
	{
		PFDLOG("PFGI", "CLSC", "ShaderConverter - can NOT read file  " << filenameIn);
		return SystemCodes::ERROR_NOFILE;
	}
	

	char line[4096];
	std::string content = "";
	std::string tempStr = "";

	while (!fio.eof())
	{
		fio >> line;
		if (line[0] == '/' && line[1] == '/')	// throw away line if its comment
			std::getline(fio, tempStr);
		else								// else get string and connect with other using @ char
		{
			content += line;
			content += "@";	// remove white space
		}
	}

	// close file
	fio.close();


	//	-----------------------------------------------------------------------
	//		Find Passes and vertex functions names
	//	-----------------------------------------------------------------------

	std::vector<HLSL_Pass> passes;


	EUINT32 techniquePosition = content.find("technique", 0);
	std::string techniqueVersion = "";
	std::string techniqueName = "";

	for (EUINT32 i = 0; i<11; ++i)
		techniqueName += content[i + techniquePosition];

	EUINT32 passPosition = content.find("pass", techniquePosition);
	std::string passName = "";

	for (EUINT32 i = 4; i<10; ++i)
		passName += content[i + passPosition];


	PFDLOG("PFGI", "CLSC", "PASS name " << passName);

	HLSL_Pass pass;


	// find vertex shader function
	EUINT32 vertexShaderPassPosition = content.find("SetVertexShader", passPosition);
	vertexShaderPassPosition = content.find("CompileShader", vertexShaderPassPosition);
	vertexShaderPassPosition = content.find("(", vertexShaderPassPosition);

	EUINT32 dotPosition = content.find(",", vertexShaderPassPosition);

	for (EUINT32 i = vertexShaderPassPosition + 1; i<dotPosition; ++i)
		pass.vsVersion += content[i];

	EUINT32 leftParenthesis = content.find("(", dotPosition + 1);


	for (EUINT32 i = dotPosition + 1; i<leftParenthesis; ++i)
	{
		if (content[i] == '@')
			continue;
		pass.vsFunctionName += content[i];
	}

	//printf("\nVertexShaderFunction name : %s \n", m_vertexShaderFunctionName.c_str());

	EUINT32 pixelShaderPassPosition = content.find("SetPixelShader", passPosition);
	pixelShaderPassPosition = content.find("CompileShader", pixelShaderPassPosition);
	pixelShaderPassPosition = content.find("(", pixelShaderPassPosition);

	dotPosition = content.find(",", pixelShaderPassPosition);

	for (EUINT32 i = pixelShaderPassPosition + 1; i<dotPosition; ++i)
		pass.psVersion += content[i];

	leftParenthesis = content.find("(", dotPosition + 1);


	for (EUINT32 i = dotPosition + 1; i<leftParenthesis; ++i)
	{
		if (content[i] == '@')
			continue;
		pass.psFunctionName += content[i];
	}

	PFDLOG("PFGI", "CLSC", "PASS VS function name " << pass.vsFunctionName);
	PFDLOG("PFGI", "CLSC", "PASS PS function name " << pass.psFunctionName);

	passes.push_back(pass);

	//	-----------------------------------------------------------------------
	//		Find Layouts
	//	-----------------------------------------------------------------------
	std::vector<HLSL_VSLayoutVariable> vsLayouts;


	EUINT32 pos = content.find(passes[0].vsFunctionName);
	// odszukaj lewy nawias, po ktorym bedzie sie zaczynal nazwa struktury
	// i spacje gdzie bedzie koniec nazwy struktury
	EUINT32 leftParenthesisPosition = content.find("(", pos + 16);
	EUINT32 rightParenthesisPosition = content.find(")", pos + 16);

	std::string vertexTypeName = "struct@";

	for (EUINT32 i = leftParenthesisPosition + 1; i <rightParenthesisPosition; ++i)
	{
		if (content[i] == '@')
			break;
		vertexTypeName += content[i];
	}

	//printf("\n nazwa struktury vertexa : '%s' \n", vertexTypeName.c_str());

	// odszukaj strukture vertex shadera
	EUINT32 vsStructPosition = content.find(vertexTypeName);

	// wyszukaj nawiasy klamrowe obejmujace strukture
	leftParenthesisPosition = content.find("{", vsStructPosition);
	rightParenthesisPosition = content.find("}", leftParenthesisPosition + 1);

	pos = leftParenthesisPosition + 1;

	EUINT32 semanticsCount = 0;	 // count of variables




	while (pos<rightParenthesisPosition - 2)
	{
		pos += 1; // ominiecie znaku @

				  // znajdz pozycje dwukropka i srednika
		EUINT32 posDot = content.find(":", pos);
		EUINT32 posComa = content.find(";", pos);

		// odczytaj typ
		std::string type = "";

		for (EUINT32 j = pos; j < posComa; ++j)
		{
			if (content[pos] == '@') break;
			type += content[pos];
			++pos;
		}
		++pos; // ominiecie @

		// odczytaj nazwe zmiennej
		std::string variableName = "";

		for (EUINT32 j = pos; j < posComa; ++j)
		{
			if (content[pos] == '@') break;
			variableName += content[pos];
			++pos;
		}

		++pos; // ominiecie @
		++pos; // omienie : znajdujacego sie po @
		++pos; // ominiecie kolejnego @

		// odczytaj semantyke
		std::string semantic = "";
		std::string semanticIndex = "";

		for (EUINT32 j = pos; j < posComa; ++j)
		{
			if (content[pos] == '@') break;

			if ((content[pos] >= '0') && (content[pos] <= '9'))
				semanticIndex += content[pos];
			else
				semantic += content[pos];

			++pos;
		}


		++pos; // ominiecie @





		HLSL_VSLayoutVariable layout;

		layout.type = type ;
		layout.name = variableName;
		layout.semantic = 0;

		if (semantic.compare("POSITION") == 0)
		{
			layout.semantic = GraphicSemantic::POSITION;
			layout.semanticFName = "POSITION";
		}
		else if (semantic.compare("COLOR") == 0)
		{
			layout.semantic = GraphicSemantic::COLOR;
			layout.semanticFName = "COLOR";
		}
		else if (semantic.compare("TEXCOORD") == 0)
		{
			layout.semantic = GraphicSemantic::TEXCOORD;
			layout.semanticFName = "TEXCOORD";
		}
		else if (semantic.compare("NORMAL") == 0)
		{
			layout.semantic = GraphicSemantic::NORMAL;
			layout.semanticFName = "NORMAL";
		}
		else if (semantic.compare("TANGENT") == 0)
		{
			layout.semantic = GraphicSemantic::TANGENT;
			layout.semanticFName = "TANGENT";
		}
		else if (semantic.compare("BITANGENT") == 0)
		{
			layout.semantic = GraphicSemantic::BITANGENT;
			layout.semanticFName = "BITANGENT";
		}
		else if (semantic.compare("BINORMAL") == 0)
		{
			layout.semantic = GraphicSemantic::BINORMAL;
			layout.semanticFName = "BINORMAL";
		}
		else
		{
			layout.semantic = GraphicSemantic::UNDEFINIED;
			layout.semanticFName = "UNDEFINIED";

			//printf("\n sc n = %s", semantic.c_str());
		}

		std::string::size_type sz;   // alias of size_t
		layout.semanticIndex = std::stoi(semanticIndex, &sz);


		//format
		layout.format = GraphicsFormat::FORMAT_R32G32B32A32_FLOAT;

		if(semantic.compare("TEXCOORD") == 0)
			layout.format = GraphicsFormat::FORMAT_R32G32_FLOAT;
		else if (semantic.compare("POSITION") == 0)
			layout.format = GraphicsFormat::FORMAT_R32G32B32_FLOAT;
		else if (semantic.compare("COLOR") == 0)
			layout.format = GraphicsFormat::FORMAT_R32G32B32A32_FLOAT;


		PFDLOG("PFGI", "CLSC", "type " << layout.type);
		PFDLOG("PFGI", "CLSC", "variable name" << layout.name);
		PFDLOG("PFGI", "CLSC", "semantic " << layout.semantic);
		PFDLOG("PFGI", "CLSC", "semantic index " << layout.semanticIndex);
		PFDLOG("PFGI", "CLSC", "semantic format " << layout.format);

		vsLayouts.push_back(layout);

		++semanticsCount;	// one more SEMANTIC
	}






	//	--------------------------------------------------------
	//		find pixel shader input layout
	//	--------------------------------------------------------
	
	// NOOOOOOOOOOOOOOOOOOOOO NEEEEEEEEEEEEEEEEEEEEEEEEEEEED



	//	--------------------------------------------------------
	//		Count cbuffers
	//	--------------------------------------------------------

	EUINT32 cbuffersCount = 1;	// start with 1 - global variables are gruped in cbuffer
	EUINT32 depth = 0;				// depth of {. Globals are at level 0


	for (EUINT32 i = 0; i<content.length(); ++i)
	{
		if (content[i] == '{') depth++;

		if (depth == 0)
		{
			if (content[i] == '@')
				continue; // printf("\n")
			else
			{
				if (content.find("cbuffer@", i) == i)	// search for cbuffer
				{
					cbuffersCount++;
				}
			}
		}

		if (content[i] == '}') depth--;
	}


	PFDLOG("PFGI", "CLSC", "CBuffers count : " << cbuffersCount);

	//	--------------------------------------------------------
	//		Create CBuffer for globals variables
	//	--------------------------------------------------------
	std::vector<HLSL_CBuffer> cbuffers;

	HLSL_CBuffer cbufferGlobal;
	cbufferGlobal.name = "GLOBAL";
	cbufferGlobal.number = 0;

	EUINT32 cBufferGlobalParmatersCount = 0;		// numberof all parametrs in all cbuffers
	
	HLSL_Variable varTmp;

	std::vector<HLSL_Variable> resources;
	std::vector<HLSL_Variable> samplers;

	HLSL_CBuffer cbufferTmp;


	depth = 0;

	EUINT32 circleBracesDepth = 0;

	std::string varName = "";
	std::string varType = "";

	for (EUINT32 i = 0; i<content.length(); ++i)
	{
		if (content[i] == '{') depth++;
		if (content[i] == '(') circleBracesDepth++;

		if ((depth == 0) && (circleBracesDepth == 0))
		{
			if (content[i] == '@')
				continue; // printf("\n")
			else
			{
				if (content.find("float@", i) == i)
				{
					cBufferGlobalParmatersCount++;
					varType = "float";
					varName = content.substr(i + 6, content.find(";", i + 5) - (i + 6));

					varTmp.name = varName;
					varTmp.type = GraphicsType::FLOAT1;
					varTmp.key = ParabellumUtils::Math::GenerateHash(varName.c_str());

					cbufferGlobal.variables.push_back(varTmp);
				}
				else if (content.find("float2@", i) == i)
				{
					cBufferGlobalParmatersCount++;
					varType = "float2";
					varName = content.substr(i + 7, content.find(";", i + 6) - (i + 7));

					varTmp.name = varName;
					varTmp.type = GraphicsType::FLOAT2;
					varTmp.key = ParabellumUtils::Math::GenerateHash(varName.c_str());

					cbufferGlobal.variables.push_back(varTmp);
				}
				else if (content.find("float3@", i) == i)
				{
					cBufferGlobalParmatersCount++;
					varType = "float3";
					varName = content.substr(i + 7, content.find(";", i + 6) - (i + 7));

					varTmp.name = varName;
					varTmp.type = GraphicsType::FLOAT3;
					varTmp.key = ParabellumUtils::Math::GenerateHash(varName.c_str());

					cbufferGlobal.variables.push_back(varTmp);
				}
				else if (content.find("float4@", i) == i)
				{
					EUINT32 char1 = content.find(";", i + 6);
					EUINT32 char2 = content.find("(", i + 6);

					varType = "float4";
					varName = content.substr(i + 7, content.find(";", i + 6) - (i + 7));

					varName = varName.substr(0, varName.find("(", 0));

					cBufferGlobalParmatersCount++;

					varTmp.name = varName;
					varTmp.type = GraphicsType::FLOAT4;
					varTmp.key = ParabellumUtils::Math::GenerateHash(varName.c_str());

					cbufferGlobal.variables.push_back(varTmp);

				}
				else if (content.find("matrix@", i) == i)
				{
					cBufferGlobalParmatersCount++;
					varType = "float4x4";
					varName = content.substr(i + 7, content.find(";", i + 6) - (i + 7));

					varTmp.name = varName;
					varTmp.type = GraphicsType::FLOAT4X4;
					varTmp.key = ParabellumUtils::Math::GenerateHash(varName.c_str());

					cbufferGlobal.variables.push_back(varTmp);
				}
				else if (content.find("float4x4@", i) == i)
				{
					cBufferGlobalParmatersCount++;
					varType = "float4x4";
					varName = content.substr(i + 9, content.find(";", i + 8) - (i + 9));

					varTmp.name = varName;
					varTmp.type = GraphicsType::FLOAT4X4;
					varTmp.key = ParabellumUtils::Math::GenerateHash(varName.c_str());

					cbufferGlobal.variables.push_back(varTmp);
				}
				else if (content.find("Texture2D@", i) == i)
				{
					if (content.find(";", i + 5) < content.find(":", i + 5))
						varName = content.substr(i + 10, content.find(";", i + 9) - (i + 10));
					else
						varName = content.substr(i + 10, content.find(":", i + 9) - (i + 10));

					varTmp.name = varName;
					varTmp.type = GraphicsType::TEXTURE2D;

					resources.push_back(varTmp);
				}
				else if (content.find("SamplerState@", i) == i)
				{
					varName = content.substr(i + 13, content.find(";", i + 12) - (i + 13));

					varTmp.name = varName;
					samplers.push_back(varTmp);
				}



			}
		}



		if (content[i] == '}') depth--;
		if (content[i] == ')') circleBracesDepth--;

	}

	cbuffers.push_back(cbufferGlobal);

	// create variables
	//printf("\n\n\n    Global CBuffer :");

	/*
	for (EUINT32 i = 0; i<cBufferGlobalParmatersCount; ++i)
	{
		printf("\n  %s \t %u \t %u", cbufferGlobal.variables[i].name.c_str(), cbufferGlobal.variables[i].type, cbufferGlobal.variables[i].key);
	}
	*/

	//	--------------------------------------------------------
	//		Get all cbuffers
	//	--------------------------------------------------------
	depth = 0;
	std::string tmpStr = "";

	int currentBuffer = 1;

	

	for (EUINT32 i = 0; i<content.length(); ++i)
	{
		if (content[i] == '{') depth++;

		if (depth == 0)
		{
			if (content[i] == '@')
				continue; // printf("\n")
			else
			{

				if (content.find("cbuffer@", i) == i)	// search for cbuffer
				{
					// get cbuffer name
					tmpStr = content.substr(i + 8, content.find("@", i + 9) - (i + 8));

					//printf("\nCbufferq name : %s \n", tmpStr.c_str());

					cbufferTmp.name = tmpStr;
					cbufferTmp.number = currentBuffer;
					cbufferTmp.variables.clear();

					// get cbuffer variables
					EUINT32 leftBraces = content.find("{", i + 9);
					EUINT32 rightBraces = content.find("}", i + 9);


					for (EUINT32 j = leftBraces; j<rightBraces - 1; ++j)
					{
						std::string type = "";
						std::string name = "";
						EUINT32 size = 0;
						// get type
						EUINT32 fc = content.find("@", j) + 1;
						EUINT32 sc = content.find("@", fc);

						type = content.substr(fc, sc - fc);

						//printf("\nvar : \t %s ", type.c_str());

						// get name
						fc = sc + 1;
						sc = content.find(";", fc + 1);

						name = content.substr(fc, sc - fc);

						//printf("%s \t  (%i)", name.c_str(), size);


						varTmp.name = name;
						varTmp.type = 0;
						varTmp.key = ParabellumUtils::Math::GenerateHash(name.c_str());

						if (type.compare("float") == 0)
							varTmp.type = GraphicsType::FLOAT1;
						else if (type.compare("float2") == 0)
							varTmp.type = GraphicsType::FLOAT2;
						else if (type.compare("float3") == 0)
							varTmp.type = GraphicsType::FLOAT3;
						else if (type.compare("float4x4") == 0)
							varTmp.type = GraphicsType::FLOAT4X4;
						else if (type.compare("matrix") == 0)
							varTmp.type = GraphicsType::FLOAT4X4;



						cbufferTmp.variables.push_back(varTmp);

						// jump far in string
						j = sc;
					}


					cbuffers.push_back(cbufferTmp);

					// next cbuffer
					++currentBuffer;
				}
			}
		}

		if (content[i] == '}') depth--;
	}



	for (EUINT32 i = 0; i<cbuffers.size(); ++i)
	{
		for (EUINT32 j = 0; j < cbuffers[i].variables.size(); ++j)
		{
			PFDLOG("PFGI", "CLSC", "name : " << cbuffers[i].variables[j].name.c_str() << "\ttype " << cbuffers[i].variables[j].type << "\tkey" << cbuffers[i].variables[j].key);
		}
		
	}



	//	--------------------------------------------------------
	//		Get samplers
	//	--------------------------------------------------------
	depth = 0;

	EUINT32 samplersCount = 0;

	std::string samplerName = "";

	for (EUINT32 i = 0; i<content.length(); ++i)
	{
		if (content[i] == '{') depth++;

		if (depth == 0)
		{
			if (content[i] == '@')
				continue; // printf("\n")
			else
			{

				if (content.find("SamplerState@", i) == i)	// search for cbuffer
				{

					EUINT32 lb = 0;
					EUINT32 le = 0;

					// find register no
					for (EUINT32 j = i; j < content.length(); ++j)
					{
						if (content[j] == ':')
							lb = j;

						if (content[j] == ';')
						{
							le = j;

							if (lb != 0 && le>lb)
							{
								samplerName = content.substr(lb + 1, le - lb);
								i = j + 1;
								samplersCount++;
							}

							break;
						}


					}

				}
			}
		}

		if (content[i] == '}') depth--;
	}




	//	--------------------------------------------------------
	//		Save In GOOD format :)
	//	--------------------------------------------------------


	FILE* fconv = fopen(filenameOut, "wb");

	//
	// Header
	//

	EUINT32 shaderType = 2;
	EUINT32 shaderVersion = 5;
	EUINT32 converterId = 7;
	EUINT32 converterFlag = 0;
	EUINT32 shaderOptions = 0;
	std::string sFilenameIn = filenameIn;

	fwrite(&shaderType, sizeof(EUINT32), 1, fconv);
	fwrite(&shaderVersion, sizeof(EUINT32), 1, fconv);
	fwrite(&converterId, sizeof(EUINT32), 1, fconv);
	fwrite(&converterFlag, sizeof(EUINT32), 1, fconv);
	fwrite(&shaderOptions, sizeof(EUINT32), 1, fconv);

	SaveString(sFilenameIn, fconv);

	// save passes

	EUINT32 passesCount = 1; //passes.size();
	fwrite(&passesCount, sizeof(EUINT32), 1, fconv);

	for (EUINT32 i = 0; i < passesCount; ++i)
	{
		SaveString(passes[i].vsFunctionName, fconv);
		SaveString(passes[i].psFunctionName, fconv);
		SaveString(passes[i].gsFunctionName, fconv);
		SaveString(passes[i].tsFunctionName, fconv);
	}


	// save layout
	EUINT32 layoutSize = vsLayouts.size();
	fwrite(&layoutSize, sizeof(EUINT32), 1, fconv);

	for (EUINT32 i = 0; i < vsLayouts.size(); ++i)
	{
		SaveString(vsLayouts[i].type, fconv);
		SaveString(vsLayouts[i].semanticFName, fconv);//SaveString(vsLayouts[i].name, fconv);
		fwrite(&vsLayouts[i].semantic, sizeof(EUINT32), 1, fconv);
		fwrite(&vsLayouts[i].semanticIndex, sizeof(EUINT32), 1, fconv);
		fwrite(&vsLayouts[i].format, sizeof(EUINT32), 1, fconv);
	}

	// save cbuffers
	cbuffersCount = cbuffers.size();
	fwrite(&cbuffersCount, sizeof(EUINT32), 1, fconv);

	for (EUINT32 i = 0; i < cbuffers.size(); ++i)
	{
		SaveString(cbuffers[i].name, fconv);

		EUINT32 vaiablesCount = cbuffers[i].variables.size();
		fwrite(&vaiablesCount, sizeof(EUINT32), 1, fconv);

		for (EUINT32 j = 0; j < vaiablesCount; ++j)
		{
			SaveString(cbuffers[i].variables[j].name, fconv);
			fwrite(&cbuffers[i].variables[j].type, sizeof(EUINT32), 1, fconv);
		}
	}


	// save resources
	EUINT32 resourcesCount = resources.size();
	fwrite(&resourcesCount, sizeof(EUINT32), 1, fconv);


	for (EUINT32 i = 0; i < resourcesCount; ++i)
	{
		SaveString(resources[i].name, fconv);
		fwrite(&resources[i].type, sizeof(EUINT32), 1, fconv);
	}


	// save samplers
	//EUINT32 samplersCount = samplers.size();
	fwrite(&samplersCount, sizeof(EUINT32), 1, fconv);

	for (EUINT32 i = 0; i < samplersCount; ++i)
	{
		SaveString(samplers[i].name, fconv);
		fwrite(&samplers[i].type, sizeof(EUINT32), 1, fconv);
	}


	fclose(fconv);




	return SystemCodes::SUCCESS;
}