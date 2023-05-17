class Camera
{
private:
	float x, y, z, alpha, betta;

public: 
	Camera(float _x, float _y, float _z, float _alpha, float _betta)
	{
		x = _x; y = _y; z = _z; alpha = _alpha; betta = _betta;
	};

	void Walk(float k)
	{
		x += sin(betta)*k;
		y += sin(alpha)*k;
		z += cos(alpha)*cos(betta)*k;
	};

	void Strafe(float k)
	{
		x += sin(betta + D3DX_PI/2)*k;
		z += cos(alpha)*cos(betta + D3DX_PI/2)*k;
	};

	void Fly(float k) {	y += k;};
	void Heading(float angle) {	if (abs(alpha + angle) < D3DX_PI/2 ) alpha += angle;};
	void Bank(float angle)    { if (abs(betta + angle) < D3DX_PI/2 ) betta += angle;};

	D3DXMATRIX* GetMatrix()
	{
		D3DXMATRIX matCam;
		return D3DXMatrixLookAtLH(&matCam, &D3DXVECTOR3(x, y, z), &D3DXVECTOR3(x + sin(betta), y + sin(alpha), z + cos(alpha)*cos(betta)), &D3DXVECTOR3(0,1,0));
	}
};