class Sphere
{
public: 
	
	float x, y, z, v, mass, R, speed;
	char type;

	Sphere() { x = y = z = v = mass = R = speed = 0;};

	Sphere(float _x, float _y, float _z, float _R, float _speed, float _v, char _type)
	{
		x = _x;
		y = _y;
		z = _z;
		speed = _speed;
		v = _v;
		R = _R;
		mass = 1 + R*R;
		type = _type;
	};

	void Move()
	{
		float a, Xv, Yv;
		if (x > 0) a = atan((z+35)/(x+35)) + D3DX_PI; else a = atan(z/x);

		Xv = speed*cos(v) + 0.001f * cos(a);
		Yv = speed*sin(v) + 0.001f * sin(a);

		if (((Xv>0)&(Yv>0))|((Xv>0)&(Yv<0))) v= atan(Yv/Xv); else v= atan(Yv/Xv) + D3DX_PI;
		speed = sqrt(Xv*Xv + Yv*Yv);
		
		x += speed*cos(v);
		z += speed*sin(v);
	};

	bool IfCollapse(Sphere *S)
	{
		if (sqrt((x - S->x)*(x - S->x) + (z - S->z)*(z - S->z) + (y - S->y)*(y - S->y) ) <= R + S->R) return true;
			else return false;
	};

	bool IfCollapse(float _x, float _z, float _r)
	{
		if (sqrt((x - _x)*(x - _x) + (z - _z)*(z - _z)) <= R + _r) return true;
			else return false;
	};

	void Collapse(Sphere *S)
	{
		float Vx1, Vx2, Vz1, Vz2, V1x1, V1z1, V1x2, V1z2, kx, a1, a2;

		if (S->x - x != 0) kx = (S->z - z)/(S->x - x); else kx = 999999;
		a1 = abs(atan(kx) - v);
		a2 = abs(atan(kx) - S->v);

		Vx1 = speed*cos(a1);
		Vx2 = S->speed*cos(a2);
		V1z1 = speed*sin(a1);
		V1z2 = S->speed*sin(a2);

		x -= speed*cos(v);
		S->x -= S->speed*cos(S->v);
		z -= speed*sin(v);
		S->z -= S->speed*sin(S->v);

		V1x1 = ((mass - S->mass)*Vx1 + 2*S->mass*Vx2)/(mass + S->mass);
		V1x2 = ((S->mass - mass)*Vx2 + 2*mass*Vx1)/(mass + S->mass);

		Vz1 = V1x1*sin(atan(kx)) + V1z1*cos(atan(kx));
		Vz2 = V1x2*sin(atan(kx)) + V1z2*cos(atan(kx));
		Vx1 = V1x1*cos(atan(kx)) - V1z1*sin(atan(kx));
		Vx2 = V1x2*cos(atan(kx)) - V1z2*sin(atan(kx));

		if (((Vx1>0)&(Vz1>0))|((Vx1>0)&(Vz1<0))) v= atan(Vz1/Vx1);
                                                   else v= atan(Vz1/Vx1) + D3DX_PI;
		if (((Vx2>0)&(Vz2>0))|((Vx2>0)&(Vz2<0))) S->v= atan(Vz2/Vx2);
                                                   else S->v= atan(Vz2/Vx2) + D3DX_PI;

		z +=  Vz1*2;
		S->z +=  Vz2*2;
		x +=  Vx1*2;
		S->x +=  Vx2*2;


		speed = sqrt(Vx1*Vx1 + Vz1*Vz1);
		S->speed = sqrt(Vx2*Vx2 + Vz2*Vz2);
	};


	void Draw(LPDIRECT3DDEVICE9 Videocard, ID3DXMesh *SphereMesh)
	{
		D3DXMATRIX matScaling, matTranslation, matWorld;

		D3DXMatrixScaling(&matScaling, R, R, R);
		D3DXMatrixTranslation(&matTranslation, x, y, z);
		D3DXMatrixMultiply(&matWorld, &matScaling, &matTranslation);	
		Videocard->SetTransform(D3DTS_WORLD, &matWorld);

		SphereMesh->DrawSubset(0);
	};
};