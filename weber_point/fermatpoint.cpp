#include "fermatpoint.h"

//---------------------------------------------------------------------------
QPointF FermatPoint::CalcFermatPoint(const QPointF& a, const QPointF& b, const QPointF& c)
{
    double X1;   //點1之X座標
    double Y1;   //點1之Y座標
    double X2;   //點2之X座標
    double Y2;   //點2之Y座標
    double X3;   //點3之X座標
    double Y3;   //點3之Y座標
    double MidX12;  //點12之X座標中點
    double MidY12;  //點12之Y座標中點
    double MidX23;  //點12之X座標中點
    double MidY23;  //點12之Y座標中點
    double M12v;     //與線段12垂直斜率
    double L12;     //線段12長度
    double b12v;    //與線段12垂直的b
    double M23v;     //與線段12垂直斜率
    double L23;     //線段12長度
    double b23v;    //與線段12垂直的b
    double temp1;
    double temp2;
    double temp3;
    double Xa;    //12邊得出新點a'之X座標
    double Ya;    //12邊得出新點a'之Y座標
    double Xb;    //12邊得出新點a'之X座標
    double Yb;    //12邊得出新點a'之Y座標
    double Xatmp1;
    double Yatmp1;
    double Xatmp2;
    double Yatmp2;
    double Ma3;   //a跟3的斜率
    double ba3;   //a跟3的b
    double Mb1;   //a跟3的斜率
    double bb1;   //a跟3的b
    double fermatX;
    double fermatY;
    double temp;

    X1 = a.x() ;
    Y1 = a.y() ;
    X2 = b.x() ;
    Y2 = b.y() ;
    X3 = c.x() ;
    Y3 = c.y() ;

    MidX12 = ( X1 + X2 ) /2;
    MidY12 = ( Y1 + Y2 ) /2;

    L12 = sqrt ( pow( X1 - X2 , 2 ) + pow( Y1 - Y2 ,2 ) );

    if( (Y2 - Y1 ) == 0 ){
         Xatmp1 = MidX12;
         Xatmp2 = MidX12;
         if( (MidX12 - X1 )>0)
             temp = MidX12 - X1;
         else
             temp = X1 - MidX12;

         Yatmp1 = Y2 +  temp * 1.732;
         Yatmp2 = Y2 -  temp * 1.732;

         if((( pow ( Xatmp1 - X3 , 2 ) + pow ( Yatmp1 - Y3 , 2 )) > ( pow ( Xatmp2 - X3 , 2 ) + pow ( Yatmp2 - Y3 , 2 ))))
          {
            Xa = Xatmp1;
            Ya = Yatmp1;
          }
         else
          {
            Xa = Xatmp2;
            Ya = Yatmp2;
          }

         }
    else{
         M12v = ( X1 - X2 ) / ( Y2 - Y1 );
         b12v = (( pow( Y2 , 2 ) + pow( X2 , 2 ) - pow( Y1 , 2 ) - pow( X1 , 2 ) ) / ( 2 * ( Y2 - Y1 )));

         temp1 = 1 + pow ( M12v , 2 );
         temp2 = (-2) * MidX12 + 2 * M12v * b12v - 2 * M12v * MidY12 ;
         temp3 = pow( MidX12 , 2 ) + pow( b12v , 2 ) - 2 * b12v * MidY12 + pow( MidY12 , 2 ) - ( pow( L12 , 2 ) * 3 / 4 );

         Xatmp1 = ((-1) * temp2 + sqrt ( pow( temp2 , 2 ) - 4 * temp1 * temp3 )) / ( 2 * temp1 );
         Xatmp2 = ((-1) * temp2 - sqrt ( pow( temp2 , 2 ) - 4 * temp1 * temp3 )) / ( 2 * temp1 );

         Yatmp1 = M12v * Xatmp1 + b12v;
         Yatmp2 = M12v * Xatmp2 + b12v;

         if((( pow ( Xatmp1 - X3 , 2 ) + pow ( Yatmp1 - Y3 , 2 )) > ( pow ( Xatmp2 - X3 , 2 ) + pow ( Yatmp2 - Y3 , 2 ))))
           {
             Xa = Xatmp1;
             Ya = Yatmp1;
           }
         else
             {
             Xa = Xatmp2;
             Ya = Yatmp2;
             }
         }

    if((X3-Xa) == 0)
    {
        Ma3 = 9999999 ;
        ba3 = 999999 ;
    }
    else
    {
        Ma3 = ( Y3 - Ya ) / ( X3 - Xa );
        ba3 = (( X3 * Ya ) - ( Xa * Y3 )) / ( X3 - Xa );
    }

    MidX23 = ( X2 + X3 ) /2;
    MidY23 = ( Y2 + Y3 ) /2;
    L23 = sqrt ( pow( X2 - X3 , 2 ) + pow( Y2 - Y3 ,2 ) );

    if( Y3 - Y2 == 0){
         Xatmp1 = MidX23;
         Xatmp2 = MidX23;
         if( (MidX23 - X2 )>0)
             temp = MidX23 - X2;
         else
             temp = X2 - MidX23;

         Yatmp1 = Y2 +  temp * 1.732;
         Yatmp2 = Y2 -  temp * 1.732;

         if((( pow ( Xatmp1 - X1 , 2 ) + pow ( Yatmp1 - Y1 , 2 )) > ( pow ( Xatmp2 - X1 , 2 ) + pow ( Yatmp2 - Y1 , 2 ))))
          {
            Xb = Xatmp1;
            Yb = Yatmp1;
          }
         else
          {
            Xb = Xatmp2;
            Yb = Yatmp2;
          }
    }
    else{
         M23v = ( X2 - X3 ) / ( Y3 - Y2 );
         b23v = (( pow( Y3 , 2 ) + pow( X3 , 2 ) - pow( Y2 , 2 ) - pow( X2 , 2 ) ) / ( 2 * ( Y3 - Y2 )));

         temp1 = 1 + pow ( M23v , 2 );

         temp2 = (-2) * MidX23 + 2 * M23v * b23v - 2 * M23v * MidY23 ;

         temp3 = pow( MidX23 , 2 ) + pow( b23v , 2 ) - 2 * b23v * MidY23 + pow( MidY23 , 2 ) - ( pow( L23 , 2 ) * 3 / 4 );

         Xatmp1 = ((-1) * temp2 + sqrt ( pow( temp2 , 2 ) - 4 * temp1 * temp3 )) / ( 2 * temp1 );
         Xatmp2 = ((-1) * temp2 - sqrt ( pow( temp2 , 2 ) - 4 * temp1 * temp3 )) / ( 2 * temp1 );

         Yatmp1 = M23v * Xatmp1 + b23v;
         Yatmp2 = M23v * Xatmp2 + b23v;

         if((( pow ( Xatmp1 - X1 , 2 ) + pow ( Yatmp1 - Y1 , 2 )) > ( pow ( Xatmp2 - X1 , 2 ) + pow ( Yatmp2 - Y1 , 2 ))))
         {
            Xb = Xatmp1;
            Yb = Yatmp1;
         }
         else
         {
            Xb = Xatmp2;
            Yb = Yatmp2;
         }
    }

    if((X1-Xb) == 0)
    {
        Mb1 = 9999999 ;
        bb1 = 9999999 ;
    }
    else
    {
        Mb1 = ( Y1 - Yb ) / ( X1 - Xb );
        bb1 = (( X1 * Yb ) - ( Xb * Y1 )) / ( X1 - Xb );
    }

    fermatX = ( bb1 - ba3 ) / ( Ma3 - Mb1 ) ;
    fermatY = ( Ma3 * bb1 - Mb1 * ba3 ) / ( Ma3 - Mb1 ) ;
    return QPointF(fermatX, fermatY);
}

QPointF FermatPoint::CalcFermatPoint(const Triangle& t)
{
    return CalcFermatPoint(t.points[0], t.points[1], t.points[2]);
}
