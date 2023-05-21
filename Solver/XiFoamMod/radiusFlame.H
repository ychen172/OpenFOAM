Info << "\nReading radiusFlame.H file\n" << endl;


volVectorField  centres = mesh.C();
// scalar summationRho = 0.0;
scalar radiusMinRho = 0.0;
const scalar coeff = 3./(4.*Foam::constant::mathematical::pi);

scalar  summationRho = sum(mesh.V()*rho*(1.0-b)) / min(rho).value();


// forAll(centres,k)
// {
//     summationRho += (mesh.V()[k]*rho[k]*(scalar(1.)-b[k])) / (min(rho).value());
// }
radiusMinRho = Foam::pow(coeff*summationRho,(1./3.));
Info << "\nRadiusMinRho = \n" << radiusMinRho << endl;
