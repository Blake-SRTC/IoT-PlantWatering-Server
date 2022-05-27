FROM node:17

RUN mkdir -p /usr/src/app

WORKDIR /usr/src/app

COPY package*.json ./

RUN npm install

COPY . .

EXPOSE 4050

# CMD ["npm", "start"]
CMD ["npm", "run", "dev"]