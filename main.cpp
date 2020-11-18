#include <iostream>
#include <graphics.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#define MAX 150
#define SONSUZ  1e18


typedef struct Nokta {
double x;
double y;
} Nokta;

typedef struct Cember {
Nokta n;
double r;
} Cember;

// kapsayan en küçük çember
Cember mec = {{0,0},SONSUZ};

int noktaSayisi;

// dosyadaki tüm noktaları tutan dizi
Nokta nkt[MAX];


//iki nokta arasındaki uzaklik
double ikiNoktaArasindakiUzaklik(Nokta a,Nokta b)
{
    return sqrt(pow(a.x - b.x, 2) + pow(a.y - b.y, 2));
}

// iki noktayı kapsayan çember
Cember cemberIkiNoktaIle(Nokta A, Nokta B)
{

    Nokta C = { (A.x + B.x) / 2.0, (A.y + B.y) / 2.0 };

    Cember D = { C, (ikiNoktaArasindakiUzaklik(A, B) / 2.0) };


    return D;
}


// üç noktayı kapsayan çember
Cember cemberUcNoktaIle(Nokta A, Nokta B, Nokta C)
{
    double mx, my, nx, ny;
    mx = B.x - A.x;
    my = B.y - A.y;
    nx = C.x - A.x;
    ny = C.y - A.y;

    double M = mx * mx + my * my;
    double N = nx * nx + ny * ny;
    double P = mx * ny - my * nx;

     Nokta merkezNokta = { (ny * M - my * N) / (2 * P),(mx * N - nx * M) / (2 * P) };

     merkezNokta.x += A.x;
     merkezNokta.y += A.y;

    Cember c = { merkezNokta, ikiNoktaArasindakiUzaklik(merkezNokta, A) };
    return c;
}

//çember noktaları kapsıyor mu kontrolü
int cemberKapsiyorMu(Cember c, Nokta n[])
{
    int i;
    for(i=0;i<noktaSayisi;i++)
    {
        int icindeMi = 0;
        if(ikiNoktaArasindakiUzaklik(c.n,n[i])<=c.r)
        {
            icindeMi =  1;
        }

        if(icindeMi == 0)
        {
            return 0;
            break;
        }


    }
    return 1;
}




Cember MEC()
 {


    if (noktaSayisi == 0)
        return { { 0, 0 }, 0 };
    if (noktaSayisi == 1)
        return { nkt[0], 0 };


    // iki nokta ile kesişen ve  tüm noktaları kapsayan cember
    for (int i = 0; i < noktaSayisi; i++) {
        for (int j = i + 1; j < noktaSayisi; j++) {


            Cember temp = cemberIkiNoktaIle(nkt[i], nkt[j]);

            if (temp.r < mec.r && cemberKapsiyorMu(temp,nkt) == 1)
             mec = temp;

        }
    }

     // üç nokta ile kesişen ve  tüm noktaları kapsayan cember
    for (int i = 0; i < noktaSayisi; i++) {
        for (int j = i + 1; j < noktaSayisi; j++) {
            for (int k = j + 1; k < noktaSayisi; k++) {


                Cember temp = cemberUcNoktaIle(nkt[i], nkt[j], nkt[k]);

                if (temp.r < mec.r && cemberKapsiyorMu(temp,nkt) == 1)
                    mec = temp;
            }
        }
    }

    return mec;
}

void BSpline(Nokta &nokta1, Nokta &nokta2, Nokta &nokta3,Nokta &nokta4, double u,Nokta &bSpline)
{
     double u2 = u * u;
     double u3 = u2 * u;
     double birEksiU = 1.0 - u;
     double mu3 = birEksiU * birEksiU * birEksiU;

     double bEksiBir = mu3;
     double bSifir = 3 * u3 - 6 * u2 + 4;
     double bBir =-3 * u3 + 3 * u2 + 3 * u + 1;
     double bIki  = u3;

    bSpline.x = (nokta1.x * bEksiBir +nokta2.x * bSifir +nokta3.x * bBir + nokta4.x * bIki)/6.0;
    bSpline.y = (nokta1.y * bEksiBir +nokta2.y * bSifir +nokta3.y * bBir +nokta4.y * bIki)/6.0;
}


void DosyaOku(char *dosyaAdi)
{
    FILE *filePtr = NULL;
    int  i = 0;
    double noktax, noktay;

    if ((filePtr = fopen(dosyaAdi, "r")) == NULL)
    {
        printf("Dosya bulunamadi\n");
        exit(EXIT_FAILURE);
    }

    while (fscanf(filePtr, "%lf %lf", &noktax,&noktay) != EOF)
    {
        nkt[i].x = noktax;
        nkt[i].y=noktay;
        i++;
    }
    noktaSayisi = i;

    fclose(filePtr);
}



int main()
{

    char *dosyaAdi = "deneme.txt";

    DosyaOku(dosyaAdi);


    int i = 0;

    for (i = 0; i < noktaSayisi; i++)
    {
        printf("%d. noktanin x ve y koordinati : (%.1f, %.1f)\n",i+1,nkt[i].x,nkt[i].y);

    }

    // grafik ekranının ayarlama
   initwindow(1200,1000,"En Küçük Çember ve B-Spline Problemi",100,50,false,true);


    //x ve y koordinat düzlemi
   line(getmaxx()/2,0,getmaxx()/2,getmaxy());
   line(0,getmaxy()/2,getmaxx(),getmaxy()/2);

   // noktalar
   for(i=0;i<noktaSayisi;i++)
   {
       setcolor(RED);
       circle(nkt[i].x+getmaxx()/2, nkt[i].y+getmaxy()/2, 2);
       floodfill(nkt[i].x+getmaxx()/2, nkt[i].y+getmaxy()/2, 4);

   }

   delay(2000);



    // kapsayan en küçük çemberi bulan fonksiyon
    Cember kapsayancember = MEC();

    printf("%\n\n Tum noktalari kapsayan cemberin koordinatlari: x:%.2f y:%.2f r:%.2f",kapsayancember.n.x,kapsayancember.n.y,kapsayancember.r);

    // kapsayan en küçük çemberin graphics.h'da çizimi
    circle(kapsayancember.n.x+getmaxx()/2,kapsayancember.n.y+getmaxy()/2,kapsayancember.r);

    delay(2000);

    // çemberin merkez ve yarıçap bilgilerinin grafik üzerinde gösterimi
    char msg[128];
    sprintf(msg, "Cemberin merkezi(x,y)= (%.1f, %.1f) Yaricapi(r) = %.1f", kapsayancember.n.x,kapsayancember.n.y,kapsayancember.r);
    line(kapsayancember.n.x+getmaxx()/2,kapsayancember.n.y+getmaxy()/2, kapsayancember.n.x+getmaxx()/2+kapsayancember.r,kapsayancember.n.y+getmaxy()/2);
    setcolor(YELLOW);
    line((kapsayancember.n.x+getmaxx()/2+kapsayancember.n.x+getmaxx()/2+kapsayancember.r)/2,kapsayancember.n.y+getmaxy()/2,kapsayancember.n.x+getmaxx()/2+100,kapsayancember.n.y+getmaxy()/2+100);
    outtextxy(kapsayancember.n.x+getmaxx()/2+100,kapsayancember.n.y+getmaxy()/2+100, msg);

    delay(4000);




    //b-spline
    Nokta bSplineNokta;

    double n1 = (double)noktaSayisi;
    setcolor(GREEN);

    for(double u = 2.0; u < n1; u += 0.001)
    {
    int u1 = (int)u;
    BSpline(nkt[u1 - 3], nkt[u1 - 2], nkt[u1 - 1], nkt[u1], u - u1, bSplineNokta);
    circle(bSplineNokta.x+getmaxx()/2, bSplineNokta.y+getmaxy()/2, 1);
    }

    setcolor(YELLOW);
    line(bSplineNokta.x+getmaxx()/2,bSplineNokta.y+getmaxy()/2,bSplineNokta.x+getmaxx()/2-500,bSplineNokta.y+getmaxy()/2+100);
    outtextxy(bSplineNokta.x+getmaxx()/2-500,bSplineNokta.y+getmaxy()/2+100, "B-SPLINE EGRISI");

    getch();
    closegraph();


    exit(EXIT_SUCCESS);


}
