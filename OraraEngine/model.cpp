
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <shlwapi.h>
#pragma comment(lib, "shlwapi.lib")

#include "main.h"
#include "rendererManager.h"
#include "material.h"
#include "model.h"



void Model::Draw()
{

	// 頂点バッファ設定
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	RendererManager::Instance().GetDeviceContext()->IASetVertexBuffers(0, 1, &m_VertexBuffer, &stride, &offset);

	// インデックスバッファ設定
    RendererManager::Instance().GetDeviceContext()->IASetIndexBuffer(m_IndexBuffer, DXGI_FORMAT_R32_UINT, 0);

	// プリミティブトポロジ設定
    RendererManager::Instance().GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);


	for( unsigned int i = 0; i < m_SubsetNum; i++ )
	{
		// マテリアル設定
        Material material;
        material.SetMaterial(m_SubsetArray[i].Material.ModelMaterial);

		// テクスチャ設定
		if(m_SubsetArray[i].Material.Texture)
            RendererManager::Instance().GetDeviceContext()->PSSetShaderResources( 0, 1, &m_SubsetArray[i].Material.Texture );

		// ポリゴン描画
        RendererManager::Instance().GetDeviceContext()->DrawIndexed( m_SubsetArray[i].IndexNum, m_SubsetArray[i].StartIndex, 0 );
	}

}



void Model::Load( const char *FileName )
{
	
	m_File = (char*)FileName;
	MODEL model;
	LoadObj( FileName, &model );



	// 頂点バッファ生成
	{
		D3D11_BUFFER_DESC bd;
		ZeroMemory( &bd, sizeof(bd) );
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof( VERTEX_3D ) * model.VertexNum;
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.CPUAccessFlags = 0;

		D3D11_SUBRESOURCE_DATA sd;
		ZeroMemory( &sd, sizeof(sd) );
		sd.pSysMem = model.VertexArray;

        RendererManager::Instance().GetDevice()->CreateBuffer( &bd, &sd, &m_VertexBuffer );
	}


	// インデックスバッファ生成
	{
		D3D11_BUFFER_DESC bd;
		ZeroMemory( &bd, sizeof(bd) );
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof( unsigned int ) * model.IndexNum;
		bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
		bd.CPUAccessFlags = 0;

		D3D11_SUBRESOURCE_DATA sd;
		ZeroMemory( &sd, sizeof(sd) );
		sd.pSysMem = model.IndexArray;

        RendererManager::Instance().GetDevice()->CreateBuffer( &bd, &sd, &m_IndexBuffer );
	}

	// サブセット設定
	{
		m_SubsetArray = new SUBSET[ model.SubsetNum ];
		m_SubsetNum = model.SubsetNum;

		for( unsigned int i = 0; i < model.SubsetNum; i++ )
		{
			m_SubsetArray[i].StartIndex = model.SubsetArray[i].StartIndex;
			m_SubsetArray[i].IndexNum = model.SubsetArray[i].IndexNum;

			m_SubsetArray[i].Material.ModelMaterial = model.SubsetArray[i].Material.ModelMaterial;

			m_SubsetArray[i].Material.Texture = NULL;

			D3DX11CreateShaderResourceViewFromFile(RendererManager::Instance().GetDevice(),
				model.SubsetArray[i].Material.TextureName,
				NULL,
				NULL,
				&m_SubsetArray[i].Material.Texture,
				NULL);

			if (m_SubsetArray[i].Material.Texture)
				m_SubsetArray[i].Material.ModelMaterial.TextureEnable = true;
			else
				m_SubsetArray[i].Material.ModelMaterial.TextureEnable = false;

		}
	}

	delete[] model.VertexArray;
	delete[] model.IndexArray;
	delete[] model.SubsetArray;

}





void Model::Unload()
{
	if (m_VertexBuffer)
		m_VertexBuffer->Release();

	if (m_IndexBuffer)
		m_IndexBuffer->Release();

	for (unsigned int i = 0; i < m_SubsetNum; i++)
	{
		if (m_SubsetArray[i].Material.Texture)
			m_SubsetArray[i].Material.Texture->Release();
	}

	delete[] m_SubsetArray;

}





//モデル読込////////////////////////////////////////////
void Model::LoadObj( const char *FileName, MODEL *Model )
{
	char dir[MAX_PATH];
	strcpy (dir, FileName );
	PathRemoveFileSpec(dir);

	D3DXVECTOR3	*positionArray;
	D3DXVECTOR3	*normalArray;
	D3DXVECTOR2	*texcoordArray;

	unsigned int	positionNum = 0;
	unsigned int	normalNum = 0;
	unsigned int	texcoordNum = 0;
	unsigned int	vertexNum = 0;
	unsigned int	indexNum = 0;
	unsigned int	in = 0;
	unsigned int	subsetNum = 0;

	MODEL_MATERIAL	*materialArray = NULL;
	unsigned int	materialNum = 0;

	char str[256];
	char *s;
	char c;


	FILE *file;
	file = fopen( FileName, "rt" );
	assert(file);



	//要素数カウント
	while( true )
	{
		fscanf( file, "%s", str );

		if( feof( file ) != 0 )
			break;

		if( strcmp( str, "v" ) == 0 )
		{
			positionNum++;
		}
		else if( strcmp( str, "vn" ) == 0 )
		{
			normalNum++;
		}
		else if( strcmp( str, "vt" ) == 0 )
		{
			texcoordNum++;
		}
		else if( strcmp( str, "usemtl" ) == 0 )
		{
			subsetNum++;
		}
		else if( strcmp( str, "f" ) == 0 )
		{
			in = 0;

			do
			{
				fscanf( file, "%s", str );
				vertexNum++;
				in++;
				c = fgetc( file );
			}
			while( c != '\n' && c!= '\r' );

			//四角は三角に分割
			if( in == 4 )
				in = 6;

			indexNum += in;
		}
	}


	//メモリ確保
	positionArray = new D3DXVECTOR3[ positionNum ];
	normalArray = new D3DXVECTOR3[ normalNum ];
	texcoordArray = new D3DXVECTOR2[ texcoordNum ];


	Model->VertexArray = new VERTEX_3D[ vertexNum ];
	Model->VertexNum = vertexNum;

	Model->IndexArray = new unsigned int[ indexNum ];
	Model->IndexNum = indexNum;

	Model->SubsetArray = new SUBSET[ subsetNum ];
	Model->SubsetNum = subsetNum;




	//要素読込
	D3DXVECTOR3 *position = positionArray;
	D3DXVECTOR3 *normal = normalArray;
	D3DXVECTOR2 *texcoord = texcoordArray;

	unsigned int vc = 0;
	unsigned int ic = 0;
	unsigned int sc = 0;


	fseek( file, 0, SEEK_SET );

	while( true )
	{
		fscanf( file, "%s", str );

		if( feof( file ) != 0 )
			break;

		if( strcmp( str, "mtllib" ) == 0 )
		{
			//マテリアルファイル
			fscanf( file, "%s", str );

			char path[256];
			strcpy( path, dir );
			strcat( path, "\\" );
			strcat( path, str );

			LoadMaterial( path, &materialArray, &materialNum );
		}
		else if( strcmp( str, "o" ) == 0 )
		{
			//オブジェクト名
			fscanf( file, "%s", str );
		}
		else if( strcmp( str, "v" ) == 0 )
		{
			//頂点座標
			fscanf( file, "%f", &position->x );
			fscanf( file, "%f", &position->y );
			fscanf( file, "%f", &position->z );
			position++;
		}
		else if( strcmp( str, "vn" ) == 0 )
		{
			//法線
			fscanf( file, "%f", &normal->x );
			fscanf( file, "%f", &normal->y );
			fscanf( file, "%f", &normal->z );
			normal++;
		}
		else if( strcmp( str, "vt" ) == 0 )
		{
			//テクスチャ座標
			fscanf( file, "%f", &texcoord->x );
			fscanf( file, "%f", &texcoord->y );
			texcoord->y = 1.0f - texcoord->y;
			texcoord++;
		}
		else if( strcmp( str, "usemtl" ) == 0 )
		{
			//マテリアル
			fscanf( file, "%s", str );

			if( sc != 0 )
				Model->SubsetArray[ sc - 1 ].IndexNum = ic - Model->SubsetArray[ sc - 1 ].StartIndex;

			Model->SubsetArray[ sc ].StartIndex = ic;


			for( unsigned int i = 0; i < materialNum; i++ )
			{
				if( strcmp( str, materialArray[i].Name ) == 0 )
				{
					Model->SubsetArray[ sc ].Material.ModelMaterial = materialArray[i].ModelMaterial;
					strcpy( Model->SubsetArray[ sc ].Material.TextureName, materialArray[i].TextureName );
					strcpy( Model->SubsetArray[ sc ].Material.Name, materialArray[i].Name );

					break;
				}
			}

			sc++;
			
		}
		else if( strcmp( str, "f" ) == 0 )
		{
			//面
			in = 0;

			do
			{
				fscanf( file, "%s", str );

				s = strtok( str, "/" );	
				Model->VertexArray[vc].Position = positionArray[ atoi( s ) - 1 ];
				if( s[ strlen( s ) + 1 ] != '/' )
				{
					//テクスチャ座標が存在しない場合もある
					s = strtok( NULL, "/" );
					Model->VertexArray[vc].TexCoord = texcoordArray[ atoi( s ) - 1 ];
				}
				s = strtok( NULL, "/" );	
				Model->VertexArray[vc].Normal = normalArray[ atoi( s ) - 1 ];

				Model->VertexArray[vc].Diffuse = D3DXVECTOR4( 1.0f, 1.0f, 1.0f, 1.0f );

				Model->IndexArray[ic] = vc;
				ic++;
				vc++;

				in++;
				c = fgetc( file );
			}
			while( c != '\n' && c != '\r' );

			//接ベクトルと従法線の作成

			CalcTangentAndBinormal(&Model->VertexArray[vc - in + 0].Position, &Model->VertexArray[vc - in + 0].TexCoord,
				&Model->VertexArray[vc - in + 1].Position, &Model->VertexArray[vc - in + 1].TexCoord,
				&Model->VertexArray[vc - in + 2].Position, &Model->VertexArray[vc - in + 2].TexCoord,
				&Model->VertexArray[vc - in + 0].Tangent, &Model->VertexArray[vc - in + 0].Binormal);

			//他の頂点にもコピーする
			Model->VertexArray[vc - in + 1].Tangent = Model->VertexArray[vc - in + 0].Tangent;
			Model->VertexArray[vc - in + 1].Binormal = Model->VertexArray[vc - in + 0].Binormal;
			Model->VertexArray[vc - in + 2].Tangent = Model->VertexArray[vc - in + 0].Tangent;
			Model->VertexArray[vc - in + 2].Binormal = Model->VertexArray[vc - in + 0].Binormal;

			//四角は三角に分割
			if( in == 4 )
			{
				//ポリゴンが四角形だった場合は頂点４にもコピーする
				Model->VertexArray[vc - in + 3].Tangent = Model->VertexArray[vc - in + 0].Tangent;
				Model->VertexArray[vc - in + 3].Binormal = Model->VertexArray[vc - in + 0].Binormal;

				Model->IndexArray[ic] = vc - 4;
				ic++;
				Model->IndexArray[ic] = vc - 2;
				ic++;
			}
		}
	}


	if( sc != 0 )
		Model->SubsetArray[ sc - 1 ].IndexNum = ic - Model->SubsetArray[ sc - 1 ].StartIndex;


	fclose( file );


	delete[] positionArray;
	delete[] normalArray;
	delete[] texcoordArray;
	delete[] materialArray;
}




//マテリアル読み込み///////////////////////////////////////////////////////////////////
void Model::LoadMaterial( const char *FileName, MODEL_MATERIAL **MaterialArray, unsigned int *MaterialNum )
{

	char dir[MAX_PATH];
	strcpy(dir, FileName);
	PathRemoveFileSpec(dir);



	char str[256];

	FILE *file;
	file = fopen( FileName, "rt" );
	assert(file);

	MODEL_MATERIAL *materialArray;
	unsigned int materialNum = 0;

	//要素数カウント
	while( true )
	{
		fscanf( file, "%s", str );

		if( feof( file ) != 0 )
			break;


		if( strcmp( str, "newmtl" ) == 0 )
		{
			materialNum++;
		}
	}


	//メモリ確保
	materialArray = new MODEL_MATERIAL[ materialNum ];


	//要素読込
	int mc = -1;

	fseek( file, 0, SEEK_SET );

	while( true )
	{
		fscanf( file, "%s", str );

		if( feof( file ) != 0 )
			break;


		if( strcmp( str, "newmtl" ) == 0 )
		{
			//マテリアル名
			mc++;
			fscanf( file, "%s", materialArray[ mc ].Name );
			strcpy( materialArray[ mc ].TextureName, "" );

			materialArray[mc].ModelMaterial.Emission.r = 0.0f;
			materialArray[mc].ModelMaterial.Emission.g = 0.0f;
			materialArray[mc].ModelMaterial.Emission.b = 0.0f;
			materialArray[mc].ModelMaterial.Emission.a = 0.0f;
		}
		else if( strcmp( str, "Ka" ) == 0 )
		{
			//アンビエント
			fscanf( file, "%f", &materialArray[ mc ].ModelMaterial.Ambient.r );
			fscanf( file, "%f", &materialArray[ mc ].ModelMaterial.Ambient.g );
			fscanf( file, "%f", &materialArray[ mc ].ModelMaterial.Ambient.b );
			materialArray[ mc ].ModelMaterial.Ambient.a = 1.0f;
		}
		else if( strcmp( str, "Kd" ) == 0 )
		{
			//ディフューズ
			fscanf( file, "%f", &materialArray[ mc ].ModelMaterial.Diffuse.r );
			fscanf( file, "%f", &materialArray[ mc ].ModelMaterial.Diffuse.g );
			fscanf( file, "%f", &materialArray[ mc ].ModelMaterial.Diffuse.b );
			materialArray[ mc ].ModelMaterial.Diffuse.a = 1.0f;
		}
		else if( strcmp( str, "Ks" ) == 0 )
		{
			//スペキュラ
			fscanf( file, "%f", &materialArray[ mc ].ModelMaterial.Specular.r );
			fscanf( file, "%f", &materialArray[ mc ].ModelMaterial.Specular.g );
			fscanf( file, "%f", &materialArray[ mc ].ModelMaterial.Specular.b );
			materialArray[ mc ].ModelMaterial.Specular.a = 1.0f;
		}
		else if( strcmp( str, "Ns" ) == 0 )
		{
			//スペキュラ強度
			fscanf( file, "%f", &materialArray[ mc ].ModelMaterial.Shininess );
		}
		else if( strcmp( str, "d" ) == 0 )
		{
			//アルファ
			fscanf( file, "%f", &materialArray[ mc ].ModelMaterial.Diffuse.a );
		}
		else if( strcmp( str, "map_Kd" ) == 0 )
		{
			//テクスチャ
			fscanf( file, "%s", str );

			char path[256];
			strcpy( path, dir );
			strcat( path, "\\" );
			strcat( path, str );

			strcat( materialArray[ mc ].TextureName, path );
		}
	}

	fclose(file);

	*MaterialArray = materialArray;
	*MaterialNum = materialNum;
}


void Model::CalcTangentAndBinormal(D3DXVECTOR3* p0, D3DXVECTOR2* uv0,
	D3DXVECTOR3* p1, D3DXVECTOR2* uv1,
	D3DXVECTOR3* p2, D3DXVECTOR2* uv2,
	D3DXVECTOR3* outTangent, D3DXVECTOR3* outBinormal)
{
	D3DXVECTOR3 CP0[3] = {
		D3DXVECTOR3(p0->x,uv0->x,uv0->y),
		D3DXVECTOR3(p0->y,uv0->x,uv0->y),
		D3DXVECTOR3(p0->z,uv0->x,uv0->y),
	};
	D3DXVECTOR3 CP1[3] = {
		D3DXVECTOR3(p1->x,uv1->x,uv1->y),
		D3DXVECTOR3(p1->y,uv1->x,uv1->y),
		D3DXVECTOR3(p1->z,uv1->x,uv1->y),
	};
	D3DXVECTOR3 CP2[3] = {
		D3DXVECTOR3(p2->x,uv2->x,uv2->y),
		D3DXVECTOR3(p2->y,uv2->x,uv2->y),
		D3DXVECTOR3(p2->z,uv2->x,uv2->y),
	};

	float U[3], V[3];
	for (int i = 0; i < 3; i++)
	{
		D3DXVECTOR3 V1 = CP1[i] - CP0[i];
		D3DXVECTOR3 V2 = CP2[i] - CP1[i];
		D3DXVECTOR3 ABC;

		D3DXVec3Cross(&ABC, &V1, &V2);

		if (ABC.x == 0.0f)
		{
			*outTangent = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			*outBinormal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			return;
		}

		U[i] = -ABC.y / ABC.x;
		V[i] = -ABC.z / ABC.x;
	}

	*outTangent = D3DXVECTOR3(U[0], U[1], U[2]);
	*outBinormal = D3DXVECTOR3(V[0], V[1], V[2]);

	D3DXVec3Normalize(outTangent, outTangent);
	D3DXVec3Normalize(outBinormal, outBinormal);

}


